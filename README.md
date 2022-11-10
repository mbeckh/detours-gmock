# detours-gmock
Helper functions and macros for using detours with GoogleTest gMock.

[![Release](https://img.shields.io/github/v/release/mbeckh/detours-gmock?display_name=tag&sort=semver&label=Release&style=flat-square)](https://github.com/mbeckh/detours-gmock/releases/)
[![Tests](https://img.shields.io/github/workflow/status/mbeckh/detours-gmock/Build/master?label=Tests&logo=GitHub&style=flat-square)](https://github.com/mbeckh/detours-gmock/actions)
[![Coverage](https://img.shields.io/codecov/c/gh/mbeckh/detours-gmock/master?label=Coverage&logo=codecov&style=flat-square)](https://codecov.io/gh/mbeckh/detours-gmock)
[![Codacy Grade](https://img.shields.io/codacy/grade/3558456c948c48a7990e169210b51f0f?label=Code%20Quality&logo=codacy&style=flat-square)](https://www.codacy.com/manual/mbeckh/detours-gmock?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=mbeckh/detours-gmock&amp;utm_campaign=Badge_Grade)
[![License](https://img.shields.io/github/license/mbeckh/detours-gmock?label=License&style=flat-square)](https://github.com/mbeckh/detours-gmock/blob/master/LICENSE)

## Description
This header-only library contains some macro magic for setting up mock classes and API wrappers using
[Microsoft Detours](https://github.com/microsoft/Detours) for unit tests with [GoogleTest gMock](https://github.com/google/googletest).

## Usage
The following code shows a sample for a test setup that intercepts calls to the functions `CreateFileW`, `WriteFile`
and `CloseHandle`of the Windows API and checks that at least a given number of bytes has been written to a file.

```cpp
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <detours_gmock.h>

#include <windows.h>

namespace t = testing;

// Provide the list of functions to mock. Any macro name will do, fn_ is used as a functor.
#define WIN32_FUNCTIONS(fn_)                                                                                                                                                                   \
  fn_(7, HANDLE, WINAPI, CreateFileW,                                                                                                                                                          \
    (LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile), \
    (lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile),                                                              \
    nullptr);                                                                                                                                                                                  \
  fn_(5, BOOL, WINAPI, WriteFile,                                                                                                                                                              \
    (HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped),                                                                  \
    (hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped),                                                                                                            \
    nullptr);                                                                                                                                                                                  \
  fn_(1, BOOL, WINAPI, CloseHandle,                                                                                                                                                            \
    (HANDLE hObject),                                                                                                                                                                          \
    (hObject),                                                                                                                                                                                 \
    nullptr);

// Define a test fixture.
class Foo_Test : public t::Test {
public:
  void SetUp() override {
    // Provide default actions for the functions.
    // SetLastErrorAndReturn is a short cut to return a value and set the Windows API error code.
    ON_CALL(m_mock, CreateFileW(t::HasSubstr(L"foo."), DTGM_ARG6))
      .WillByDefault(detours_gmock::SetLastErrorAndReturn(ERROR_FILE_NOT_FOUND, INVALID_HANDLE_VALUE));
    ON_CALL(m_mock, WriteFile(m_hFile, DTGM_ARG4))
      .WillByDefault(t::Invoke([](t::Unused, t::Unused, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, t::Unused) {
         *lpNumberOfBytesWritten = nNumberOfBytesToWrite;
         m_bytes += nNumberOfBytesToWrite;
         return TRUE;
      }));
    ON_CALL(m_mock, CloseHandle(m_hFile))
      .WillByDefault(t::Return(TRUE));
  }

  void TearDown() override {
    // Explicit verification before the mock is destroyed.
    t::Mock::VerifyAndClearExpectations(&m_mock);
  }

protected:
  // Instantiate mock object m_mock and intercept calls.
  DTGM_API_MOCK(m_mock, WIN32_FUNCTIONS);
  // Handle for use in mocked calls. Not a _real_ handle though so don't use in any actual API calls.
  HANDLE m_hFile = &m_mock;
  DWORD m_bytes = 0;
};

TEST_F(Foo_Test, Bar) {
  // Set up number of expected calls, override default behavior if required.
  EXPECT_CALL(m_mock, CreateFileW(t::HasSubstr(L"foo.txt"), DTGM_ARG6))
    .WillOnce(t::Return(m_hFile));
  EXPECT_CALL(m_mock, WriteFile(m_hFile, DTGM_ARG4));
  EXPECT_CALL(m_mock, CloseHandle(m_hFile));

  // call some code which opens the file foo.txt and writes to it
  myfunction();

  // check that at least 100 bytes have been written to the file
  EXPECT_LE(100, m_bytes);
}
```

## License
The code is released under the Apache License Version 2.0. Please see [LICENSE](LICENSE) for details.
