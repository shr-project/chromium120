#!/usr/bin/env python3
# Copyright (C) 2023 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License a
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from python.generators.diff_tests.testing import Path, DataPath, Metric
from python.generators.diff_tests.testing import Csv, Json, TextProto
from python.generators.diff_tests.testing import DiffTestBlueprint
from python.generators.diff_tests.testing import TestSuite


class StdlibCommon(TestSuite):

  def test_thread_state_summary(self):
    return DiffTestBlueprint(
        trace=Path('../../common/synth_1.py'),
        query="""
        INCLUDE PERFETTO MODULE common.thread_states;

        SELECT
          state,
          cpu,
          dur
        FROM thread_state_summary_for_interval(
          25,
          75,
          (
            SELECT utid
            FROM thread
            WHERE name = 'init'
          )
        )
        """,
        out=Csv("""
        "state","cpu","dur"
        "Running",1,50
        "Runnable","[NULL]",25
        """))