/*
Copyright 2019-2020 Michael Beckh

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

#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <windows.h>
// MUST include <windows.h> before <detours.h>
#include <detours.h>

#include <cassert>
#include <functional>
#include <memory>
#include <type_traits>

#define DTGM_ARG0
#define DTGM_ARG1 testing::_                                                                                                  // NOLINT(cppcoreguidelines-macro-usage)
#define DTGM_ARG2 testing::_, testing::_                                                                                      // NOLINT(cppcoreguidelines-macro-usage)
#define DTGM_ARG3 testing::_, testing::_, testing::_                                                                          // NOLINT(cppcoreguidelines-macro-usage)
#define DTGM_ARG4 testing::_, testing::_, testing::_, testing::_                                                              // NOLINT(cppcoreguidelines-macro-usage)
#define DTGM_ARG5 testing::_, testing::_, testing::_, testing::_, testing::_                                                  // NOLINT(cppcoreguidelines-macro-usage)
#define DTGM_ARG6 testing::_, testing::_, testing::_, testing::_, testing::_, testing::_                                      // NOLINT(cppcoreguidelines-macro-usage)
#define DTGM_ARG7 testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_                          // NOLINT(cppcoreguidelines-macro-usage)
#define DTGM_ARG8 testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_              // NOLINT(cppcoreguidelines-macro-usage)
#define DTGM_ARG9 testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_  // NOLINT(cppcoreguidelines-macro-usage)

#define DTGM_INTERNAL_API_ACTION0(function_) /* NOLINT(cppcoreguidelines-macro-usage) */        \
	__pragma(warning(suppress : 4100))                                                          \
		ACTION(DTGM_Call##function_) { /* NOLINT(cppcoreguidelines-special-member-functions) */ \
		return DTGM_real##function_();                                                          \
	}

#define DTGM_INTERNAL_API_ACTION1(function_) /* NOLINT(cppcoreguidelines-macro-usage) */        \
	__pragma(warning(suppress : 4100))                                                          \
		ACTION(DTGM_Call##function_) { /* NOLINT(cppcoreguidelines-special-member-functions) */ \
		return DTGM_real##function_(arg0);                                                      \
	}

#define DTGM_INTERNAL_API_ACTION2(function_) /* NOLINT(cppcoreguidelines-macro-usage) */        \
	__pragma(warning(suppress : 4100))                                                          \
		ACTION(DTGM_Call##function_) { /* NOLINT(cppcoreguidelines-special-member-functions) */ \
		return DTGM_real##function_(arg0, arg1);                                                \
	}

#define DTGM_INTERNAL_API_ACTION3(function_) /* NOLINT(cppcoreguidelines-macro-usage) */        \
	__pragma(warning(suppress : 4100))                                                          \
		ACTION(DTGM_Call##function_) { /* NOLINT(cppcoreguidelines-special-member-functions) */ \
		return DTGM_real##function_(arg0, arg1, arg2);                                          \
	}

#define DTGM_INTERNAL_API_ACTION4(function_) /* NOLINT(cppcoreguidelines-macro-usage) */        \
	__pragma(warning(suppress : 4100))                                                          \
		ACTION(DTGM_Call##function_) { /* NOLINT(cppcoreguidelines-special-member-functions) */ \
		return DTGM_real##function_(arg0, arg1, arg2, arg3);                                    \
	}

#define DTGM_INTERNAL_API_ACTION5(function_) /* NOLINT(cppcoreguidelines-macro-usage) */        \
	__pragma(warning(suppress : 4100))                                                          \
		ACTION(DTGM_Call##function_) { /* NOLINT(cppcoreguidelines-special-member-functions) */ \
		return DTGM_real##function_(arg0, arg1, arg2, arg3, arg4);                              \
	}

#define DTGM_INTERNAL_API_ACTION6(function_) /* NOLINT(cppcoreguidelines-macro-usage) */        \
	__pragma(warning(suppress : 4100))                                                          \
		ACTION(DTGM_Call##function_) { /* NOLINT(cppcoreguidelines-special-member-functions) */ \
		return DTGM_real##function_(arg0, arg1, arg2, arg3, arg4, arg5);                        \
	}

#define DTGM_INTERNAL_API_ACTION7(function_) /* NOLINT(cppcoreguidelines-macro-usage) */        \
	__pragma(warning(suppress : 4100))                                                          \
		ACTION(DTGM_Call##function_) { /* NOLINT(cppcoreguidelines-special-member-functions) */ \
		return DTGM_real##function_(arg0, arg1, arg2, arg3, arg4, arg5, arg6);                  \
	}

#define DTGM_INTERNAL_API_ACTION8(function_) /* NOLINT(cppcoreguidelines-macro-usage) */        \
	__pragma(warning(suppress : 4100))                                                          \
		ACTION(DTGM_Call##function_) { /* NOLINT(cppcoreguidelines-special-member-functions) */ \
		return DTGM_real##function_(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7);            \
	}

#define DTGM_INTERNAL_API_ACTION9(function_) /* NOLINT(cppcoreguidelines-macro-usage) */        \
	__pragma(warning(suppress : 4100))                                                          \
		ACTION(DTGM_Call##function_) { /* NOLINT(cppcoreguidelines-special-member-functions) */ \
		return DTGM_real##function_(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);      \
	}

#define DTGM_INTERNAL_DEFINE_API_MOCK_METHOD(parameterCount_, return_, callType_, function_, parameters_, arguments_, default_) /* NOLINT(cppcoreguidelines-macro-usage) */ \
	MOCK_METHOD##parameterCount_##_WITH_CALLTYPE(callType_, function_, return_ parameters_)                                     /* NOLINT(misc-non-private-member-variables-in-classes) */

#define DTGM_INTERNAL_DEFINE_API_FUNCTIONS(parameterCount_, return_, callType_, function_, parameters_, arguments_, default_) /* NOLINT(cppcoreguidelines-macro-usage) */                                                  \
	static return_(callType_* DTGM_real##function_) parameters_ = function_;                                                  /* NOLINT(bugprone-macro-parentheses, cppcoreguidelines-avoid-non-const-global-variables) */ \
	DTGM_INTERNAL_API_ACTION##parameterCount_(function_);                                                                                                                                                                  \
	return_ callType_ DTGM_My##function_ parameters_ {                                                                                                                                                                     \
		return g_pApiMock->function_ arguments_;                                                                                                                                                                           \
	}

#define DTGM_INTERNAL_SET_DEFAULT_API_ACTION(parameterCount_, return_, callType_, function_, parameters_, arguments_, default_) /* NOLINT(cppcoreguidelines-macro-usage) */ \
	do {                                                                                                                                                                    \
		auto lambda = default_;                                                                                                                                             \
		if (std::is_null_pointer_v<decltype(lambda)>) {                                                                                                                     \
			ON_CALL(*g_pApiMock, function_(DTGM_ARG##parameterCount_)).WillByDefault(DTGM_Call##function_());                                                               \
			EXPECT_CALL(*g_pApiMock, function_(DTGM_ARG##parameterCount_)).Times(testing::AnyNumber());                                                                     \
		} else {                                                                                                                                                            \
			ON_CALL(*g_pApiMock, function_(DTGM_ARG##parameterCount_)).WillByDefault(default_);                                                                             \
		}                                                                                                                                                                   \
	} while (0)

#define DTGM_INTERNAL_API_ATTACH(parameterCount_, return_, callType_, function_, parameters_, arguments_, default_) /* NOLINT(cppcoreguidelines-macro-usage) */ \
	ASSERT_EQ(NO_ERROR, DetourAttach(&reinterpret_cast<void*&>(DTGM_real##function_), reinterpret_cast<void*>(DTGM_My##function_)))

#define DTGM_INTERNAL_API_DETACH(parameterCount_, return_, callType_, function_, parameters_, arguments_, default_) /* NOLINT(cppcoreguidelines-macro-usage) */ \
	ASSERT_EQ(NO_ERROR, DetourDetach(&reinterpret_cast<void*&>(DTGM_real##function_), reinterpret_cast<void*>(DTGM_My##function_)))

#define DTGM_DECLARE_API_MOCK(name_, functions_) /* NOLINT(cppcoreguidelines-macro-usage) */              \
	namespace detours_gmock##name_ {                                                                      \
		namespace {                                                                                       \
		class DTGM_ApiMock;                                                                               \
		void DTGM_Setup();                                                                                \
		static DTGM_ApiMock* g_pApiMock; /* NOLINT(cppcoreguidelines-avoid-non-const-global-variables) */ \
		class DTGM_ApiMock {                                                                              \
		public:                                                                                           \
			DTGM_ApiMock() {                                                                              \
				g_pApiMock = this;                                                                        \
				DTGM_Setup();                                                                             \
			}                                                                                             \
			DTGM_ApiMock(const DTGM_ApiMock&) = delete;                                                   \
			DTGM_ApiMock(DTGM_ApiMock&&) = delete;                                                        \
			~DTGM_ApiMock() {                                                                             \
				g_pApiMock = nullptr;                                                                     \
			}                                                                                             \
			DTGM_ApiMock& operator=(const DTGM_ApiMock&) = delete;                                        \
			DTGM_ApiMock& operator=(DTGM_ApiMock&&) = delete;                                             \
			functions_(DTGM_INTERNAL_DEFINE_API_MOCK_METHOD); /* NOLINT(clang-diagnostic-extra-semi) */   \
		};                                                                                                \
		functions_(DTGM_INTERNAL_DEFINE_API_FUNCTIONS); /* NOLINT(clang-diagnostic-extra-semi) */         \
		void DTGM_Setup() {                                                                               \
			functions_(DTGM_INTERNAL_SET_DEFAULT_API_ACTION); /* NOLINT(clang-diagnostic-extra-semi) */   \
			ASSERT_EQ(NO_ERROR, DetourTransactionBegin());                                                \
			ASSERT_EQ(NO_ERROR, DetourUpdateThread(GetCurrentThread()));                                  \
			functions_(DTGM_INTERNAL_API_ATTACH); /* NOLINT(clang-diagnostic-extra-semi) */               \
			ASSERT_EQ(NO_ERROR, DetourTransactionCommit());                                               \
		}                                                                                                 \
		void DTGM_Detach() {                                                                              \
			ASSERT_EQ(NO_ERROR, DetourTransactionBegin());                                                \
			ASSERT_EQ(NO_ERROR, DetourUpdateThread(GetCurrentThread()));                                  \
			functions_(DTGM_INTERNAL_API_DETACH); /* NOLINT(clang-diagnostic-extra-semi) */               \
			ASSERT_EQ(NO_ERROR, DetourTransactionCommit());                                               \
		}                                                                                                 \
		}                                                                                                 \
	}

#define DTGM_DEFINE_API_MOCK(name_, var_) /* NOLINT(cppcoreguidelines-macro-usage) */ \
	detours_gmock##name_::DTGM_ApiMock var_;

#define DTGM_DEFINE_NICE_API_MOCK(name_, var_)                  /* NOLINT(cppcoreguidelines-macro-usage) */ \
	testing::NiceMock<detours_gmock##name_::DTGM_ApiMock> var_; /* NOLINT(bugprone-macro-parentheses) */

#define DTGM_DEFINE_STRICT_API_MOCK(name_, var_)                  /* NOLINT(cppcoreguidelines-macro-usage) */ \
	testing::StrictMock<detours_gmock##name_::DTGM_ApiMock> var_; /* NOLINT(bugprone-macro-parentheses) */

#define DTGM_DETACH_API_MOCK(name_) /* NOLINT(cppcoreguidelines-macro-usage) */ \
	detours_gmock##name_::DTGM_Detach()

#define DTGM_REAL(name_, function_) /* NOLINT(cppcoreguidelines-macro-usage) */ \
	detours_gmock##name_::DTGM_real##function_

//
//
//

#define DTGM_INTERNAL_DECLARE_CLASS_FAKE_METHOD(class_, parameterCount_, return_, function_, parameters_, arguments_, default_)                           /* NOLINT(cppcoreguidelines-macro-usage) */                \
	return_ DTGM_fake##function_ parameters_;                                                                                                             /* NOLINT(misc-non-private-member-variables-in-classes) */ \
	inline static return_(DTGM_FakeClass::*m_real##function_) parameters_ = reinterpret_cast<return_(DTGM_FakeClass::*) parameters_>(&class_::function_); /* NOLINT(bugprone-macro-parentheses, cppcoreguidelines-avoid-non-const-global-variables) */

#define DTGM_INTERNAL_DEFINE_CLASS_FAKE_METHOD(class_, parameterCount_, return_, function_, parameters_, arguments_, default_) /* NOLINT(cppcoreguidelines-macro-usage) */ \
	return_ DTGM_FakeClass::DTGM_fake##function_ parameters_ {                                                                                                             \
		EnterCriticalSection(&g_pClassMock->m_cs);                                                                                                                         \
		class Guard {                                                                                                                                                      \
		public:                                                                                                                                                            \
			Guard() = default;                                                                                                                                             \
			Guard(const Guard&) = delete;                                                                                                                                  \
			Guard(Guard&&) = delete;                                                                                                                                       \
			~Guard() noexcept {                                                                                                                                            \
				LeaveCriticalSection(&g_pClassMock->m_cs);                                                                                                                 \
			}                                                                                                                                                              \
			Guard& operator=(const Guard&) = delete;                                                                                                                       \
			Guard& operator=(Guard&&) = delete;                                                                                                                            \
		};                                                                                                                                                                 \
		Guard guard;                                                                                                                                                       \
		g_pClassMock->m_pObject = this;                                                                                                                                    \
		return g_pClassMock->function_ arguments_;                                                                                                                         \
	}

#define DTGM_INTERNAL_DEFINE_CLASS_MOCK_METHOD(class_, parameterCount_, return_, function_, parameters_, arguments_, default_) /* NOLINT(cppcoreguidelines-macro-usage) */                                                                           \
	MOCK_METHOD##parameterCount_(function_, return_ parameters_);                                                              /* NOLINT(cppcoreguidelines-non-private-member-variables-in-classes, misc-non-private-member-variables-in-classes) */ \
	inline static return_(DTGM_FakeClass::*m_fake##function_) parameters_ = &DTGM_FakeClass::DTGM_fake##function_;             /* NOLINT(bugprone-macro-parentheses, cppcoreguidelines-avoid-non-const-global-variables) */

#define DTGM_INTERNAL_SET_DEFAULT_CLASS_ACTION(class_, parameterCount_, return_, function_, parameters_, arguments_, default_) /* NOLINT(cppcoreguidelines-macro-usage) */                                                                                                     \
	do {                                                                                                                                                                                                                                                                       \
		auto lambda = default_;                                                                                                                                                                                                                                                \
		if (std::is_null_pointer_v<decltype(lambda)>) {                                                                                                                                                                                                                        \
			ON_CALL(*g_pClassMock, function_(DTGM_ARG##parameterCount_)).WillByDefault([] parameters_ { return (g_pClassMock->self().*static_cast<return_(class_::*) parameters_>(DTGM_FakeClass::m_real##function_)) arguments_; }); /* NOLINT(bugprone-macro-parentheses) */ \
			EXPECT_CALL(*g_pClassMock, function_(DTGM_ARG##parameterCount_)).Times(testing::AnyNumber());                                                                                                                                                                      \
		} else {                                                                                                                                                                                                                                                               \
			ON_CALL(*g_pClassMock, function_(DTGM_ARG##parameterCount_)).WillByDefault(default_);                                                                                                                                                                              \
		}                                                                                                                                                                                                                                                                      \
	} while (0)

#define DTGM_INTERNAL_CLASS_ATTACH(class_, parameterCount_, return_, function_, parameters_, arguments_, default_) /* NOLINT(cppcoreguidelines-macro-usage) */ \
	ASSERT_EQ(NO_ERROR, DetourAttach(&reinterpret_cast<void*&>(DTGM_FakeClass::m_real##function_), *reinterpret_cast<BYTE**>(&DTGM_ClassMock::m_fake##function_)));

#define DTGM_INTERNAL_CLASS_DETACH(class_, parameterCount_, return_, function_, parameters_, arguments_, default_) /* NOLINT(cppcoreguidelines-macro-usage) */ \
	ASSERT_EQ(NO_ERROR, DetourDetach(&reinterpret_cast<void*&>(DTGM_FakeClass::m_real##function_), *reinterpret_cast<BYTE**>(&DTGM_ClassMock::m_fake##function_)));

#define DTGM_DECLARE_CLASS_MOCK(class_, functions_) /* NOLINT(cppcoreguidelines-macro-usage) */                 \
	namespace detours_gmock_class_##class_ {                                                                    \
		namespace {                                                                                             \
		class DTGM_ClassMock;                                                                                   \
		void DTGM_SetupClass();                                                                                 \
		static DTGM_ClassMock* g_pClassMock;   /* NOLINT(cppcoreguidelines-avoid-non-const-global-variables) */ \
		class DTGM_FakeClass : public class_ { /* NOLINT(bugprone-macro-parentheses) */                         \
		public:                                                                                                 \
			functions_(DTGM_INTERNAL_DECLARE_CLASS_FAKE_METHOD); /* NOLINT(clang-diagnostic-extra-semi) */      \
		};                                                                                                      \
		class DTGM_ClassMock {                                                                                  \
		public:                                                                                                 \
			DTGM_ClassMock() { /* NOLINT(cppcoreguidelines-pro-type-member-init) */                             \
				InitializeCriticalSection(&m_cs);                                                               \
				g_pClassMock = this;                                                                            \
				DTGM_SetupClass();                                                                              \
			}                                                                                                   \
			DTGM_ClassMock(const DTGM_ClassMock&) = delete;                                                     \
			DTGM_ClassMock(DTGM_ClassMock&&) = delete;                                                          \
			~DTGM_ClassMock() {                                                                                 \
				g_pClassMock = nullptr;                                                                         \
				DeleteCriticalSection(&m_cs);                                                                   \
			}                                                                                                   \
			DTGM_ClassMock& operator=(const DTGM_ClassMock&) = delete;                                          \
			DTGM_ClassMock& operator=(DTGM_ClassMock&&) = delete;                                               \
			class_& self() const noexcept { /* NOLINT(bugprone-macro-parentheses) */                            \
				return *m_pObject;                                                                              \
			}                                                                                                   \
			functions_(DTGM_INTERNAL_DEFINE_CLASS_MOCK_METHOD); /* NOLINT(clang-diagnostic-extra-semi) */       \
                                                                                                                \
		private:                                                                                                \
			CRITICAL_SECTION m_cs;                                                                              \
			class_* m_pObject; /* NOLINT(bugprone-macro-parentheses) */                                         \
			friend DTGM_FakeClass;                                                                              \
		};                                                                                                      \
		functions_(DTGM_INTERNAL_DEFINE_CLASS_FAKE_METHOD); /* NOLINT(clang-diagnostic-extra-semi) */           \
		void DTGM_SetupClass() {                                                                                \
			functions_(DTGM_INTERNAL_SET_DEFAULT_CLASS_ACTION); /* NOLINT(clang-diagnostic-extra-semi) */       \
			ASSERT_EQ(NO_ERROR, DetourTransactionBegin());                                                      \
			ASSERT_EQ(NO_ERROR, DetourUpdateThread(GetCurrentThread()));                                        \
			functions_(DTGM_INTERNAL_CLASS_ATTACH); /* NOLINT(clang-diagnostic-extra-semi) */                   \
			ASSERT_EQ(NO_ERROR, DetourTransactionCommit());                                                     \
		}                                                                                                       \
		void DTGM_DetachClass() {                                                                               \
			ASSERT_EQ(NO_ERROR, DetourTransactionBegin());                                                      \
			ASSERT_EQ(NO_ERROR, DetourUpdateThread(GetCurrentThread()));                                        \
			functions_(DTGM_INTERNAL_CLASS_DETACH); /* NOLINT(clang-diagnostic-extra-semi) */                   \
			ASSERT_EQ(NO_ERROR, DetourTransactionCommit());                                                     \
		}                                                                                                       \
		}                                                                                                       \
	}

#define DTGM_DEFINE_CLASS_MOCK(class_, var_) /* NOLINT(cppcoreguidelines-macro-usage) */ \
	detours_gmock_class_##class_::DTGM_ClassMock var_;

#define DTGM_DEFINE_NICE_CLASS_MOCK(class_, var_)                         /* NOLINT(cppcoreguidelines-macro-usage) */ \
	testing::NiceMock<detours_gmock_class_##class_::DTGM_ClassMock> var_; /* NOLINT(bugprone-macro-parentheses) */

#define DTGM_DEFINE_STRICT_CLASS_MOCK(class_, var_)                         /* NOLINT(cppcoreguidelines-macro-usage) */ \
	testing::StrictMock<detours_gmock_class_##class_::DTGM_ClassMock> var_; /* NOLINT(bugprone-macro-parentheses) */

#define DTGM_DETACH_CLASS_MOCK(class_) /* NOLINT(cppcoreguidelines-macro-usage) */ \
	detours_gmock_class_##class_::DTGM_DetachClass()

namespace detours_gmock {

#pragma warning(suppress : 4100)
ACTION_P(SetLastError, lastError_) {  // NOLINT(cppcoreguidelines-special-member-functions, misc-non-private-member-variables-in-classes)
	::SetLastError(lastError_);
}

#pragma warning(suppress : 4100)
ACTION_P2(SetLastErrorAndReturn, lastError_, value_) {  // NOLINT(cppcoreguidelines-special-member-functions, misc-non-private-member-variables-in-classes)
	::SetLastError(lastError_);
	return value_;
}

#pragma warning(suppress : 4100)
ACTION_P3(WithAssert, mock, obj, action) {  // NOLINT(cppcoreguidelines-special-member-functions, misc-non-private-member-variables-in-classes)
	static_assert(std::is_same_v<std::remove_cvref_t<decltype(mock->self())>, std::remove_cvref_t<std::remove_pointer_t<obj_type>>>);
	if (std::addressof(mock->self()) != obj) {
		assert(false);
		throw std::bad_function_call();
	}
	return static_cast<testing::Action<function_type>>(action).Perform(args);
}

}  // namespace detours_gmock
