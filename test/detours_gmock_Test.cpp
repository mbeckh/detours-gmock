#/*
Copyright 2021 Michael Beckh

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "detours_gmock.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <shlwapi.h>
#include <windows.h>

namespace detours_gmock::test {

namespace t = testing;

#define WIN32_FUNCTIONS(fn_) /* NOLINT(cppcoreguidelines-macro-usage) */ \
	fn_(1, int, WINAPI, StrToIntA,                                       \
		(PCSTR pszSrc),                                                  \
		(pszSrc),                                                        \
		nullptr);

DTGM_DECLARE_API_MOCK(Win32, WIN32_FUNCTIONS);


class TestClass {
public:
	[[nodiscard]] __declspec(noinline) int GetValue() const {
		return m_value;
	};

public:
	static constexpr int kValue = 23;

private:
	int m_value = kValue;  // NOLINT()
};

#define CLASS_FUNCTIONS(fn_) /* NOLINT(cppcoreguidelines-macro-usage) */ \
	fn_(TestClass, 0, int, GetValue,                                     \
		(),                                                              \
		(),                                                              \
		nullptr);

DTGM_DECLARE_CLASS_MOCK(TestClass, CLASS_FUNCTIONS);

//
// API Mock
//

TEST(API_Test, Plain_Call_ReturnResult) {  // NOLINT(cert-err58-cpp, cppcoreguidelines-avoid-non-const-global-variables)
	EXPECT_EQ(42, StrToIntA("42"));
}

TEST(API_Test, Plain_Error_Return) {  // NOLINT(cert-err58-cpp, cppcoreguidelines-avoid-non-const-global-variables)
	SetLastError(0u);
	EXPECT_EQ(0, StrToIntA("Test"));
	EXPECT_EQ(0u, GetLastError());
}

TEST(API_Test, Mock_Call_ReturnMocked) {  // NOLINT(cert-err58-cpp, cppcoreguidelines-avoid-non-const-global-variables)
	constexpr int kResult = 42;
	DTGM_DEFINE_API_MOCK(Win32, mock);
	ON_CALL(mock, StrToIntA(t::_))
		.WillByDefault(t::Return(kResult));
	EXPECT_CALL(mock, StrToIntA(t::_)).Times(2);

	EXPECT_EQ(kResult, StrToIntA("88"));
	EXPECT_EQ(kResult, StrToIntA("Test"));

	DTGM_DETACH_API_MOCK(Win32);
}

TEST(API_Test, Mock_Call_SetLastError) {  // NOLINT(cert-err58-cpp, cppcoreguidelines-avoid-non-const-global-variables)
	constexpr DWORD kErrorCode = 99u;
	DTGM_DEFINE_API_MOCK(Win32, mock);
	ON_CALL(mock, StrToIntA(t::_))
		.WillByDefault(t::DoAll(detours_gmock::SetLastError(kErrorCode), t::Return(-1)));
	EXPECT_CALL(mock, StrToIntA(t::_));

	SetLastError(0u);
	EXPECT_EQ(-1, StrToIntA("Test"));
	EXPECT_EQ(kErrorCode, GetLastError());

	DTGM_DETACH_API_MOCK(Win32);
}

TEST(API_Test, Mock_Call_SetLastErrorAndReturn) {  // NOLINT(cert-err58-cpp, cppcoreguidelines-avoid-non-const-global-variables)
	constexpr DWORD kErrorCode = 99u;
	DTGM_DEFINE_API_MOCK(Win32, mock);
	ON_CALL(mock, StrToIntA(t::_))
		.WillByDefault(detours_gmock::SetLastErrorAndReturn(kErrorCode, -1));
	EXPECT_CALL(mock, StrToIntA(t::_));

	SetLastError(0u);
	EXPECT_EQ(-1, StrToIntA("Test"));
	EXPECT_EQ(kErrorCode, GetLastError());

	DTGM_DETACH_API_MOCK(Win32);
}

TEST(API_Test, Mock_CallReal_ReturnResult) {  // NOLINT(cert-err58-cpp, cppcoreguidelines-avoid-non-const-global-variables)
	constexpr int kMockedResult = 42;
	DTGM_DEFINE_API_MOCK(Win32, mock);
	ON_CALL(mock, StrToIntA(t::_))
		.WillByDefault(t::Return(kMockedResult));

	EXPECT_CALL(mock, StrToIntA(t::_)).Times(0);
	EXPECT_EQ(88, DTGM_REAL(Win32, StrToIntA)("88"));
	EXPECT_EQ(0, DTGM_REAL(Win32, StrToIntA)("Test"));

	DTGM_DETACH_API_MOCK(Win32);
}

TEST(API_Test, NiceMock_Call_ReturnMocked) {  // NOLINT(cert-err58-cpp, cppcoreguidelines-avoid-non-const-global-variables)
	constexpr int kResult = 42;
	DTGM_DEFINE_NICE_API_MOCK(Win32, mock);
	ON_CALL(mock, StrToIntA(t::_))
		.WillByDefault(t::Return(kResult));

	EXPECT_EQ(kResult, StrToIntA("88"));
	EXPECT_EQ(kResult, StrToIntA("Test"));

	DTGM_DETACH_API_MOCK(Win32);
}

TEST(API_Test, StrictMock_Call_ReturnMocked) {  // NOLINT(cert-err58-cpp, cppcoreguidelines-avoid-non-const-global-variables)
	constexpr int kResult = 42;
	DTGM_DEFINE_STRICT_API_MOCK(Win32, mock);
	ON_CALL(mock, StrToIntA(t::_))
		.WillByDefault(t::Return(kResult));

	EXPECT_CALL(mock, StrToIntA(t::_)).Times(2);
	EXPECT_EQ(kResult, StrToIntA("88"));
	EXPECT_EQ(kResult, StrToIntA("Test"));

	DTGM_DETACH_API_MOCK(Win32);
}


//
// Class Mock
//

TEST(Class_Test, Plain_Call_ReturnResult) {  // NOLINT(cert-err58-cpp, cppcoreguidelines-avoid-non-const-global-variables)
	TestClass tc;
	EXPECT_EQ(TestClass::kValue, tc.GetValue());
}

TEST(Class_Test, Mock_Call_ReturnMocked) {  // NOLINT(cert-err58-cpp, cppcoreguidelines-avoid-non-const-global-variables)
	constexpr int kResult = 42;
	TestClass tc;
	DTGM_DEFINE_CLASS_MOCK(TestClass, mock);
	ON_CALL(mock, GetValue())
		.WillByDefault(t::Return(kResult));
	EXPECT_CALL(mock, GetValue());

	EXPECT_EQ(kResult, tc.GetValue());

	DTGM_DETACH_CLASS_MOCK(TestClass);
}

TEST(Class_Test, Mock_WithAssert_ReturnMocked) {  // NOLINT(cert-err58-cpp, cppcoreguidelines-avoid-non-const-global-variables)
	TestClass tc;
	DTGM_DEFINE_CLASS_MOCK(TestClass, mock);
	ON_CALL(mock, GetValue())
		.WillByDefault(detours_gmock::WithAssert(&mock, &tc, t::Return(-1)));
	EXPECT_CALL(mock, GetValue());

	EXPECT_EQ(-1, tc.GetValue());

	DTGM_DETACH_CLASS_MOCK(TestClass);
}

TEST(Class_Test, Mock_WithAssertError_Abort) {  // NOLINT(cert-err58-cpp, cppcoreguidelines-avoid-non-const-global-variables)
	TestClass tc;
	TestClass other;
	DTGM_DEFINE_CLASS_MOCK(TestClass, mock);
	ON_CALL(mock, GetValue())
		.WillByDefault(detours_gmock::WithAssert(&mock, &tc, t::Return(-1)));
	EXPECT_CALL(mock, GetValue()).Times(0);

	int i = 0;
#ifdef NDEBUG
	EXPECT_DEATH(i = other.GetValue(), "");  // NOLINT(cppcoreguidelines-avoid-goto, cppcoreguidelines-pro-type-vararg)
#else
	EXPECT_DEATH(i = other.GetValue(), "Assertion failed: false");  // NOLINT(cppcoreguidelines-avoid-goto, cppcoreguidelines-pro-type-vararg)
#endif

	DTGM_DETACH_CLASS_MOCK(TestClass);
}

TEST(Class_Test, NiceMock_Call_ReturnMocked) {  // NOLINT(cert-err58-cpp, cppcoreguidelines-avoid-non-const-global-variables)
	constexpr int kResult = 42;
	TestClass tc;
	DTGM_DEFINE_NICE_CLASS_MOCK(TestClass, mock);
	ON_CALL(mock, GetValue())
		.WillByDefault(t::Return(kResult));

	EXPECT_EQ(kResult, tc.GetValue());
}

TEST(Class_Test, StrictMock_Call_ReturnMocked) {  // NOLINT(cert-err58-cpp, cppcoreguidelines-avoid-non-const-global-variables)
	constexpr int kResult = 42;
	TestClass tc;
	DTGM_DEFINE_STRICT_CLASS_MOCK(TestClass, mock);
	ON_CALL(mock, GetValue())
		.WillByDefault(t::Return(kResult));
	EXPECT_CALL(mock, GetValue());

	EXPECT_EQ(kResult, tc.GetValue());
}

}  // namespace detours_gmock::test
