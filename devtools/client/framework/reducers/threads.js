/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at <http://mozilla.org/MPL/2.0/>. */
"use strict";

const initialReducerState = {
  threads: [],
  selected: null,
};

exports.reducer = threadsReducer;
function threadsReducer(state = initialReducerState, action) {
  switch (action.type) {
    case "SELECT_THREAD": {
      return { ...state, selected: action.thread };
    }
    case "ADD_THREAD": {
      return {
        ...state,
        threads: [...state.threads, action.thread],
      };
    }
    case "CLEAR_THREAD": {
      const threads = state.threads.filter(
        thread => thread._targetFront !== action.target
      );
      return { ...state, threads };
    }
  }
  return state;
}

exports.getToolboxThreads = getToolboxThreads;
function getToolboxThreads(state) {
  return state.threads.threads;
}

exports.getSelectedThread = getSelectedThread;
function getSelectedThread(state) {
  return state.threads.selected;
}
