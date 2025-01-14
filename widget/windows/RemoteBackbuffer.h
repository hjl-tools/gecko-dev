/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef widget_windows_RemoteBackbuffer_h
#define widget_windows_RemoteBackbuffer_h

#include <thread>
#include <windows.h>
#include "mozilla/Maybe.h"

namespace mozilla {
namespace widget {
namespace remote_backbuffer {

struct SharedData;
struct BorrowResponseData;
struct PresentResponseData;
class SharedImage;
class PresentableSharedImage;

class Provider {
 public:
  Provider();
  ~Provider();

  bool Initialize(HWND aWindowHandle, DWORD aTargetProcessId);

  Maybe<RemoteBackbufferHandles> CreateRemoteHandles();

  Provider(const Provider&) = delete;
  Provider(Provider&&) = delete;
  Provider& operator=(const Provider&) = delete;
  Provider& operator=(Provider&&) = delete;

 private:
  void ThreadMain();

  void HandleBorrowRequest(BorrowResponseData* aResponseData);
  void HandlePresentRequest(PresentResponseData* aResponseData);

  HWND mWindowHandle;
  DWORD mTargetProcessId;
  HANDLE mFileMapping;
  HANDLE mRequestReadyEvent;
  HANDLE mResponseReadyEvent;
  SharedData* mSharedDataPtr;
  bool mStopServiceThread;
  std::thread mServiceThread;
  std::unique_ptr<PresentableSharedImage> mBackbuffer;
};

class Client {
 public:
  Client();
  ~Client();

  bool Initialize(const RemoteBackbufferHandles& aRemoteHandles);

  already_AddRefed<gfx::DrawTarget> BorrowDrawTarget();
  bool PresentDrawTarget();

  Client(const Client&) = delete;
  Client(Client&&) = delete;
  Client& operator=(const Client&) = delete;
  Client& operator=(Client&&) = delete;

 private:
  HANDLE mFileMapping;
  HANDLE mRequestReadyEvent;
  HANDLE mResponseReadyEvent;
  SharedData* mSharedDataPtr;
  std::unique_ptr<SharedImage> mBackbuffer;
};

}  // namespace remote_backbuffer
}  // namespace widget
}  // namespace mozilla

#endif  // widget_windows_RemoteBackbuffer_h