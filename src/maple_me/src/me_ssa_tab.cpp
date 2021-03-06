/*
 * Copyright (c) [2019-2020] Huawei Technologies Co.,Ltd.All rights reserved.
 *
 * OpenArkCompiler is licensed under the Mulan PSL v1.
 * You can use this software according to the terms and conditions of the Mulan PSL v1.
 * You may obtain a copy of Mulan PSL v1 at:
 *
 *     http://license.coscl.org.cn/MulanPSL
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
 * FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PSL v1 for more details.
 */
#include "me_ssa_tab.h"
#include <cstdlib>
#include "mpl_timer.h"

// allocate the data structure to store SSA information
namespace maple {
AnalysisResult *MeDoSSATab::Run(MeFunction *func, MeFuncResultMgr*, ModuleResultMgr*) {
  MPLTimer timer;
  timer.Start();
  if (DEBUGFUNC(func)) {
    LogInfo::MapleLogger() << "\n============== SSA and AA preparation =============" << '\n';
  }
  MemPool *memPool = NewMemPool();
  // allocate ssaTab including its SSAPart to store SSA information for statements
  auto *ssaTab = memPool->New<SSATab>(memPool, func->GetVersMp(), &func->GetMIRModule());
  func->SetMeSSATab(ssaTab);
#if DEBUG
  globalSSATab = ssaTab;
#endif
  // pass through the program statements
  for (auto bIt = func->valid_begin(); bIt != func->valid_end(); ++bIt) {
    auto *bb = *bIt;
    for (auto &stmt : bb->GetStmtNodes()) {
      ssaTab->CreateSSAStmt(stmt);  // this adds the SSANodes for exprs
    }
  }
  if (DEBUGFUNC(func)) {
    timer.Stop();
    LogInfo::MapleLogger() << "ssaTab consumes cumulatively " << timer.Elapsed() << "seconds " << '\n';
  }
  return ssaTab;
}
}  // namespace maple
