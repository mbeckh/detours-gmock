/*
Copyright 2019-2021 Michael Beckh

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

/// @file
#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <windows.h>
#include <detours.h>

#include <cassert>
#include <cstdlib>
#include <memory>
#include <type_traits>

#define DTGM_ARG0
#define DTGM_ARG1 testing::_
#define DTGM_ARG2 testing::_, testing::_
#define DTGM_ARG3 testing::_, testing::_, testing::_
#define DTGM_ARG4 testing::_, testing::_, testing::_, testing::_
#define DTGM_ARG5 testing::_, testing::_, testing::_, testing::_, testing::_
#define DTGM_ARG6 testing::_, testing::_, testing::_, testing::_, testing::_, testing::_
#define DTGM_ARG7 testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_
#define DTGM_ARG8 testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_
#define DTGM_ARG9 testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_, testing::_

#define DTGM_INTERNAL_DEFINE_API_MOCK_METHOD(parameterCount_, return_, callType_, function_, parameters_, arguments_, default_) \
public:                                                                                                                         \
	MOCK_METHOD(return_, function_, parameters_, (Calltype(callType_)));                                                        \
	return_ DTGM_Real_##function_ parameters_ {                                                                                 \
		return DTGM_Function_##function_ arguments_;                                                                            \
	}                                                                                                                           \
	/* NOLINTNEXTLINE(bugprone-macro-parentheses, cppcoreguidelines-avoid-non-const-global-variables) */                        \
	static inline return_(callType_* DTGM_Function_##function_) parameters_ = ::function_;                                      \
                                                                                                                                \
private:                                                                                                                        \
	static return_ DTGM_Call_##function_ parameters_ {                                                                          \
		return DTGM_Function_##function_ arguments_;                                                                            \
	}                                                                                                                           \
	static return_ callType_ DTGM_My_##function_ parameters_ {                                                                  \
		return s_pMock->function_ arguments_;                                                                                   \
	}

#define DTGM_INTERNAL_SET_DEFAULT_API_ACTION(parameterCount_, return_, callType_, function_, parameters_, arguments_, default_) \
	do {                                                                                                                        \
		auto lambda = (default_);                                                                                               \
		if (std::is_null_pointer_v<decltype(lambda)>) {                                                                         \
			ON_CALL(*this, function_(DTGM_ARG##parameterCount_)).WillByDefault(DTGM_Call_##function_);                          \
			EXPECT_CALL(*this, function_(DTGM_ARG##parameterCount_)).Times(testing::AnyNumber());                               \
		} else {                                                                                                                \
			ON_CALL(*this, function_(DTGM_ARG##parameterCount_)).WillByDefault((default_));                                     \
		}                                                                                                                       \
	} while (0)

#define DTGM_INTERNAL_API_ATTACH(parameterCount_, return_, callType_, function_, parameters_, arguments_, default_) \
	ASSERT_EQ(NO_ERROR, DetourAttach(&reinterpret_cast<void*&>(DTGM_Function_##function_), reinterpret_cast<void*>(DTGM_My_##function_)))

#define DTGM_INTERNAL_API_DETACH(parameterCount_, return_, callType_, function_, parameters_, arguments_, default_) \
	ASSERT_EQ(NO_ERROR, DetourDetach(&reinterpret_cast<void*&>(DTGM_Function_##function_), reinterpret_cast<void*>(DTGM_My_##function_)))

#define DTGM_DECLARE_API_MOCK(name_, functions_)                                 \
	class detours_gmock_##name_ {                                                \
	public:                                                                      \
		detours_gmock_##name_() {                                                \
			assert(!s_pMock);                                                    \
			s_pMock = this;                                                      \
			DTGM_Setup();                                                        \
		}                                                                        \
		detours_gmock_##name_(const detours_gmock_##name_&) = delete;            \
		detours_gmock_##name_(detours_gmock_##name_&&) = delete;                 \
		~detours_gmock_##name_() {                                               \
			s_pMock = nullptr;                                                   \
			DTGM_Detach();                                                       \
		}                                                                        \
		detours_gmock_##name_& operator=(const detours_gmock_##name_&) = delete; \
		detours_gmock_##name_& operator=(detours_gmock_##name_&&) = delete;      \
                                                                                 \
	public:                                                                      \
		/* NOLINTNEXTLINE(clang-diagnostic-extra-semi) */                        \
		functions_(DTGM_INTERNAL_DEFINE_API_MOCK_METHOD);                        \
                                                                                 \
	private:                                                                     \
		void DTGM_Setup() {                                                      \
			functions_(DTGM_INTERNAL_SET_DEFAULT_API_ACTION);                    \
			ASSERT_EQ(NO_ERROR, DetourTransactionBegin());                       \
			ASSERT_EQ(NO_ERROR, DetourUpdateThread(GetCurrentThread()));         \
			functions_(DTGM_INTERNAL_API_ATTACH);                                \
			ASSERT_EQ(NO_ERROR, DetourTransactionCommit());                      \
		}                                                                        \
		void DTGM_Detach() {                                                     \
			ASSERT_EQ(NO_ERROR, DetourTransactionBegin());                       \
			ASSERT_EQ(NO_ERROR, DetourUpdateThread(GetCurrentThread()));         \
			functions_(DTGM_INTERNAL_API_DETACH);                                \
			ASSERT_EQ(NO_ERROR, DetourTransactionCommit());                      \
		}                                                                        \
                                                                                 \
	private:                                                                     \
		/* NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables) */ \
		static inline detours_gmock_##name_* s_pMock = nullptr;                  \
	}

#define DTGM_DEFINE_API_MOCK(name_, var_) \
	detours_gmock_##name_ var_

#define DTGM_DEFINE_NICE_API_MOCK(name_, var_) \
	testing::NiceMock<detours_gmock_##name_> var_

#define DTGM_DEFINE_STRICT_API_MOCK(name_, var_) \
	testing::StrictMock<detours_gmock_##name_> var_

#define DTGM_DETACH_API_MOCK(name_)  // deprecated

#define DTGM_API_MOCK_HELPER(name_, var_, functions_) \
	DTGM_DECLARE_API_MOCK(name_, functions_);         \
	DTGM_DEFINE_API_MOCK(name_, var_)

#define DTGM_NICE_API_MOCK_HELPER(name_, var_, functions_) \
	DTGM_DECLARE_API_MOCK(name_, functions_);              \
	DTGM_DEFINE_NICE_API_MOCK(name_, var_)

#define DTGM_STRICT_API_MOCK_HELPER(name_, var_, functions_) \
	DTGM_DECLARE_API_MOCK(name_, functions_);                \
	DTGM_DEFINE_STRICT_API_MOCK(name_, var_)

#define DTGM_API_MOCK_NAME_HELPER(base_, id_) base_##id_
#define DTGM_API_MOCK_NAME(base_, id_) DTGM_API_MOCK_NAME_HELPER(base_, id_)

#define DTGM_API_MOCK(var_, functions_) \
	DTGM_API_MOCK_HELPER(DTGM_API_MOCK_NAME(DTGM_API_, __COUNTER__), var_, functions_)

#define DTGM_NICE_API_MOCK(var_, functions_) \
	DTGM_NICE_API_MOCK_HELPER(DTGM_API_MOCK_NAME(DTGM_API_, __COUNTER__), var_, functions_)

#define DTGM_STRICT_API_MOCK(var_, functions_) \
	DTGM_STRICT_API_MOCK_HELPER(DTGM_API_MOCK_NAME(DTGM_API_, __COUNTER__), var_, functions_)

#define DTGM_STRINGIZE(arg_) #arg_
#define DTGM_MAKE_STRING(macro_, value_) macro_(value_)

#define DTGM_REAL(name_, function_)                                                                                                        \
	__pragma(message(__FILE__ "(" DTGM_MAKE_STRING(DTGM_STRINGIZE, __LINE__) "): DTGM_REAL is deprecated, use mock.DTGM_Real_<function>")) \
	    detours_gmock_##name_::DTGM_Function_##function_

//
//
//
#define DTGM_INTERNAL_CLASS_FAKE_METHOD(class_, parameterCount_, return_, function_, parameters_, arguments_, default_)                                           \
	/* NOLINTNEXTLINE(bugprone-macro-parentheses, cppcoreguidelines-avoid-non-const-global-variables) */                                                          \
	static inline return_(DTGM_FakeClass::*DTGM_Function_##function_) parameters_ = reinterpret_cast<return_(DTGM_FakeClass::*) parameters_>(&class_::function_); \
	return_ DTGM_Fake_##function_ parameters_ {                                                                                                                   \
		EnterCriticalSection(&s_pClassMock->m_cs);                                                                                                                \
		class Guard {                                                                                                                                             \
		public:                                                                                                                                                   \
			Guard() = default;                                                                                                                                    \
			Guard(const Guard&) = delete;                                                                                                                         \
			Guard(Guard&&) = delete;                                                                                                                              \
			~Guard() noexcept {                                                                                                                                   \
				LeaveCriticalSection(&s_pClassMock->m_cs);                                                                                                        \
			}                                                                                                                                                     \
			Guard& operator=(const Guard&) = delete;                                                                                                              \
			Guard& operator=(Guard&&) = delete;                                                                                                                   \
		};                                                                                                                                                        \
		Guard guard;                                                                                                                                              \
		s_pClassMock->m_pObject = this;                                                                                                                           \
		return s_pClassMock->function_ arguments_;                                                                                                                \
	}

#define DTGM_INTERNAL_DEFINE_CLASS_MOCK_METHOD(class_, parameterCount_, return_, function_, parameters_, arguments_, default_) \
public:                                                                                                                        \
	MOCK_METHOD(return_, function_, parameters_);                                                                              \
                                                                                                                               \
private:                                                                                                                       \
	/* NOLINTNEXTLINE(bugprone-macro-parentheses, cppcoreguidelines-avoid-non-const-global-variables) */                       \
	static inline return_(DTGM_FakeClass::*DTGM_Fake_##function_) parameters_ = &DTGM_FakeClass::DTGM_Fake_##function_;

#define DTGM_INTERNAL_SET_DEFAULT_CLASS_ACTION(class_, parameterCount_, return_, function_, parameters_, arguments_, default_)                                    \
	do {                                                                                                                                                          \
		auto lambda = (default_);                                                                                                                                 \
		if (std::is_null_pointer_v<decltype(lambda)>) {                                                                                                           \
			ON_CALL(*this, function_(DTGM_ARG##parameterCount_))                                                                                                  \
			    .WillByDefault(                                                                                                                                   \
			        [this] parameters_ { /* NOLINT(bugprone-macro-parentheses) */                                                                                 \
				                         /* NOLINTNEXTLINE(bugprone-macro-parentheses) */                                                                         \
				                         return (self().*reinterpret_cast<return_(class_::*) parameters_>(DTGM_FakeClass::DTGM_Function_##function_)) arguments_; \
			        });                                                                                                                                           \
			EXPECT_CALL(*this, function_(DTGM_ARG##parameterCount_)).Times(testing::AnyNumber());                                                                 \
		} else {                                                                                                                                                  \
			ON_CALL(*this, function_(DTGM_ARG##parameterCount_)).WillByDefault((default_));                                                                       \
		}                                                                                                                                                         \
	} while (0)

#define DTGM_INTERNAL_CLASS_ATTACH(class_, parameterCount_, return_, function_, parameters_, arguments_, default_) \
	ASSERT_EQ(NO_ERROR, DetourAttach(&reinterpret_cast<void*&>(DTGM_FakeClass::DTGM_Function_##function_), *reinterpret_cast<BYTE**>(&DTGM_Fake_##function_)));

#define DTGM_INTERNAL_CLASS_DETACH(class_, parameterCount_, return_, function_, parameters_, arguments_, default_) \
	ASSERT_EQ(NO_ERROR, DetourDetach(&reinterpret_cast<void*&>(DTGM_FakeClass::DTGM_Function_##function_), *reinterpret_cast<BYTE**>(&DTGM_Fake_##function_)));

#define DTGM_DECLARE_CLASS_MOCK(class_, functions_)                                            \
	class detours_gmock_class_##class_ {                                                       \
	private:                                                                                   \
		/* NOLINTNEXTLINE(bugprone-macro-parentheses) */                                       \
		class DTGM_FakeClass : public class_ {                                                 \
		public:                                                                                \
			/* NOLINTNEXTLINE(clang-diagnostic-extra-semi) */                                  \
			functions_(DTGM_INTERNAL_CLASS_FAKE_METHOD);                                       \
		};                                                                                     \
                                                                                               \
	public:                                                                                    \
		/* NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init) */                           \
		detours_gmock_class_##class_() {                                                       \
			InitializeCriticalSection(&m_cs);                                                  \
			s_pClassMock = this;                                                               \
			DTGM_SetupClass();                                                                 \
		}                                                                                      \
		detours_gmock_class_##class_(const detours_gmock_class_##class_&) = delete;            \
		detours_gmock_class_##class_(detours_gmock_class_##class_&&) = delete;                 \
		~detours_gmock_class_##class_() {                                                      \
			s_pClassMock = nullptr;                                                            \
			DTGM_DetachClass();                                                                \
			DeleteCriticalSection(&m_cs);                                                      \
		}                                                                                      \
		detours_gmock_class_##class_& operator=(const detours_gmock_class_##class_&) = delete; \
		detours_gmock_class_##class_& operator=(detours_gmock_class_##class_&&) = delete;      \
                                                                                               \
		/* NOLINTNEXTLINE(bugprone-macro-parentheses) */                                       \
		class_& self() const noexcept {                                                        \
			return *m_pObject;                                                                 \
		}                                                                                      \
		/*  NOLINTNEXTLINE(clang-diagnostic-extra-semi) */                                     \
		functions_(DTGM_INTERNAL_DEFINE_CLASS_MOCK_METHOD);                                    \
                                                                                               \
	private:                                                                                   \
		void DTGM_SetupClass() {                                                               \
			functions_(DTGM_INTERNAL_SET_DEFAULT_CLASS_ACTION);                                \
			ASSERT_EQ(NO_ERROR, DetourTransactionBegin());                                     \
			ASSERT_EQ(NO_ERROR, DetourUpdateThread(GetCurrentThread()));                       \
			functions_(DTGM_INTERNAL_CLASS_ATTACH);                                            \
			ASSERT_EQ(NO_ERROR, DetourTransactionCommit());                                    \
		}                                                                                      \
		void DTGM_DetachClass() {                                                              \
			ASSERT_EQ(NO_ERROR, DetourTransactionBegin());                                     \
			ASSERT_EQ(NO_ERROR, DetourUpdateThread(GetCurrentThread()));                       \
			functions_(DTGM_INTERNAL_CLASS_DETACH);                                            \
			ASSERT_EQ(NO_ERROR, DetourTransactionCommit());                                    \
		}                                                                                      \
                                                                                               \
	private:                                                                                   \
		/* NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables) */               \
		static inline detours_gmock_class_##class_* s_pClassMock = nullptr;                    \
		CRITICAL_SECTION m_cs;                                                                 \
		class_* m_pObject; /* NOLINT(bugprone-macro-parentheses) */                            \
		friend class DTGM_FakeClass;                                                           \
	}

#define DTGM_DEFINE_CLASS_MOCK(class_, var_) \
	detours_gmock_class_##class_ var_

#define DTGM_DEFINE_NICE_CLASS_MOCK(class_, var_) \
	testing::NiceMock<detours_gmock_class_##class_> var_

#define DTGM_DEFINE_STRICT_CLASS_MOCK(class_, var_) \
	testing::StrictMock<detours_gmock_class_##class_> var_

#define DTGM_DETACH_CLASS_MOCK(class_)  // deprecated

#define DTGM_CLASS_MOCK(class_, var_, functions_) \
	DTGM_DECLARE_CLASS_MOCK(class_, functions_);  \
	DTGM_DEFINE_CLASS_MOCK(class_, var_)

#define DTGM_NICE_CLASS_MOCK(class_, var_, functions_) \
	DTGM_DECLARE_CLASS_MOCK(class_, functions_);       \
	DTGM_DEFINE_NICE_CLASS_MOCK(class_, var_)

#define DTGM_STRICT_CLASS_MOCK(class_, var_, functions_) \
	DTGM_DECLARE_CLASS_MOCK(class_, functions_);         \
	DTGM_DEFINE_STRICT_CLASS_MOCK(class_, var_)


namespace detours_gmock {

#pragma warning(suppress : 4100)
ACTION_P3(WithAssert, mock, obj, action) {
	static_assert(std::is_same_v<std::remove_cvref_t<decltype(mock->self())>, std::remove_cvref_t<std::remove_pointer_t<obj_type>>>);
	if (std::addressof(mock->self()) != obj) {
		assert(false);
		std::abort();
	}
	return static_cast<testing::Action<function_type>>(action).Perform(args);
}

}  // namespace detours_gmock
