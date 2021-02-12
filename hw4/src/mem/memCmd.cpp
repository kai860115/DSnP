/****************************************************************************
  FileName     [ memCmd.cpp ]
  PackageName  [ mem ]
  Synopsis     [ Define memory test commands ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <iomanip>
#include "memCmd.h"
#include "memTest.h"
#include "cmdParser.h"
#include "util.h"

using namespace std;

extern MemTest mtest;  // defined in memTest.cpp

bool
initMemCmd()
{
   if (!(cmdMgr->regCmd("MTReset", 3, new MTResetCmd) &&
         cmdMgr->regCmd("MTNew", 3, new MTNewCmd) &&
         cmdMgr->regCmd("MTDelete", 3, new MTDeleteCmd) &&
         cmdMgr->regCmd("MTPrint", 3, new MTPrintCmd)
      )) {
      cerr << "Registering \"mem\" commands fails... exiting" << endl;
      return false;
   }
   return true;
}


//----------------------------------------------------------------------
//    MTReset [(size_t blockSize)]
//----------------------------------------------------------------------
CmdExecStatus
MTResetCmd::exec(const string& option)
{
   // check option
   string token;
   if (!CmdExec::lexSingleOption(option, token))
      return CMD_EXEC_ERROR;
   if (token.size()) {
      int b;
      if (!myStr2Int(token, b) || b < int(toSizeT(sizeof(MemTestObj)))) {
         cerr << "Illegal block size (" << token << ")!!" << endl;
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
      #ifdef MEM_MGR_H
      mtest.reset(toSizeT(b));
      #else
      mtest.reset();
      #endif // MEM_MGR_H
   }
   else
      mtest.reset();
   return CMD_EXEC_DONE;
}

void
MTResetCmd::usage(ostream& os) const
{  
   os << "Usage: MTReset [(size_t blockSize)]" << endl;
}

void
MTResetCmd::help() const
{  
   cout << setw(15) << left << "MTReset: " 
        << "(memory test) reset memory manager" << endl;
}


//----------------------------------------------------------------------
//    MTNew <(size_t numObjects)> [-Array (size_t arraySize)]
//----------------------------------------------------------------------
CmdExecStatus
MTNewCmd::exec(const string& option)
{
   // TODO
   vector<string> options;
   if (!CmdExec::lexOptions(option, options))
      return CMD_EXEC_ERROR;

   if (options.empty())
      return CmdExec::errorOption(CMD_OPT_MISSING, "");

   stack<string> cmdStack;
   cmdStack.push("");
   size_t numObjects = 0;
   size_t arraySize = 0;
   
   for (auto& opt : options) {
      if (cmdStack.empty()) {
         if (myStrNCmp("-Array", opt, 2) == 0 && arraySize == 0) 
            cmdStack.push(opt);
         else 
            return CmdExec::errorOption(CMD_OPT_EXTRA, opt);
      }
      else if (cmdStack.top() == "") {
         int temp;
         if (myStrNCmp("-Array", opt, 2) == 0 && arraySize == 0) 
            cmdStack.push(opt);
         else if (myStr2Int(opt, temp)) {
            if (temp <= 0) 
               return CmdExec::errorOption(CMD_OPT_ILLEGAL, opt);
            else {
               numObjects = (size_t)temp;
               cmdStack.pop();
            }
         }
         else 
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, opt);
      }
      else {
         int temp;
         if (myStr2Int(opt, temp)) {
            if (temp <= 0)
               return CmdExec::errorOption(CMD_OPT_ILLEGAL, opt);
            else {
               arraySize = (size_t)temp;
               cmdStack.pop();
            }
         }
         else 
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, opt);
      }
   }
   if (!cmdStack.empty())
      return CmdExec::errorOption(CMD_OPT_MISSING, cmdStack.top());

   // Use try-catch to catch the bad_alloc exception
   try {
      if (arraySize != 0) 
         mtest.newArrs(numObjects, arraySize);
      else 
         mtest.newObjs(numObjects);
   }
   catch (bad_alloc exception) {
      return CMD_EXEC_ERROR;
   }
   return CMD_EXEC_DONE;
}

void
MTNewCmd::usage(ostream& os) const
{  
   os << "Usage: MTNew <(size_t numObjects)> [-Array (size_t arraySize)]\n";
}

void
MTNewCmd::help() const
{  
   cout << setw(15) << left << "MTNew: " 
        << "(memory test) new objects" << endl;
}


//----------------------------------------------------------------------
//    MTDelete <-Index (size_t objId) | -Random (size_t numRandId)> [-Array]
//----------------------------------------------------------------------
CmdExecStatus
MTDeleteCmd::exec(const string& option)
{
   // TODO
   vector<string> options;
   if (!CmdExec::lexOptions(option, options))
      return CMD_EXEC_ERROR;

   if (options.empty())
      return CmdExec::errorOption(CMD_OPT_MISSING, "");

   stack<string> cmdStack;
   cmdStack.push("");
   bool doArray = false;
   string arg;
   size_t objId = 0;
   size_t numRandId = 0;

   for (auto& opt : options) {
      if (cmdStack.empty()) {
         if (myStrNCmp("-Array", opt, 2) == 0 && !doArray) 
            doArray = true;
         else 
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, opt);
      }
      else if (cmdStack.top() == "") {
         if (myStrNCmp("-Index", opt, 2) == 0 || myStrNCmp("-Random", opt, 2) == 0) {
            cmdStack.pop();
            cmdStack.push(opt);
            arg = opt;
         }
         else if (myStrNCmp("-Array", opt, 2) == 0 && !doArray) 
            doArray = true;
         else 
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, opt);
      }
      else {
         int temp;
         if (myStr2Int(opt, temp)) {
            if (myStrNCmp("-Index", cmdStack.top(), 2) == 0) {
               if (temp < 0)
                  return CmdExec::errorOption(CMD_OPT_ILLEGAL, opt);
               objId = (size_t)temp;
            }
            else if (myStrNCmp("-Random", cmdStack.top(), 2) == 0) {
               if (temp <= 0)
                  return CmdExec::errorOption(CMD_OPT_ILLEGAL, opt);
               numRandId = (size_t)temp;
            }
            cmdStack.pop();
         }
         else 
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, opt);
      }
   }
   if (!cmdStack.empty())
      return CmdExec::errorOption(CMD_OPT_MISSING, cmdStack.top());

   if (numRandId == 0) {
      if (doArray) {
         if (objId >=  mtest.getArrListSize()) {
            cerr << "Size of array list (" << mtest.getArrListSize() << ") is <= " << objId << "!!" << endl;
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, to_string(objId));
         }
         mtest.deleteArr(objId);
      }
      else {
         if (objId >= mtest.getObjListSize()) {
            cerr << "Size of object list (" << mtest.getObjListSize() << ") is <= " << objId << "!!" << endl;
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, to_string(objId));
         }
         mtest.deleteObj(objId);
      }
   }
   else {
      if (doArray) {
         if (mtest.getArrListSize() == 0) {
            cerr << "Size of array list is 0!!\n";
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, arg);
         }
         for (size_t i = 0; i < numRandId; i++)
            mtest.deleteArr((size_t)rnGen(mtest.getArrListSize()));
      }
      else {
         if (mtest.getObjListSize() == 0) {
            cerr << "Size of object list is 0!!\n";
            return CmdExec::errorOption(CMD_OPT_ILLEGAL, arg);
         }
         for (size_t i = 0; i < numRandId; i++)
            mtest.deleteObj((size_t)rnGen(mtest.getObjListSize()));
      }
   }
   return CMD_EXEC_DONE;
}

void
MTDeleteCmd::usage(ostream& os) const
{  
   os << "Usage: MTDelete <-Index (size_t objId) | "
      << "-Random (size_t numRandId)> [-Array]" << endl;
}

void
MTDeleteCmd::help() const
{  
   cout << setw(15) << left << "MTDelete: " 
        << "(memory test) delete objects" << endl;
}


//----------------------------------------------------------------------
//    MTPrint
//----------------------------------------------------------------------
CmdExecStatus
MTPrintCmd::exec(const string& option)
{
   // check option
   if (option.size())
      return CmdExec::errorOption(CMD_OPT_EXTRA, option);
   mtest.print();

   return CMD_EXEC_DONE;
}

void
MTPrintCmd::usage(ostream& os) const
{  
   os << "Usage: MTPrint" << endl;
}

void
MTPrintCmd::help() const
{  
   cout << setw(15) << left << "MTPrint: " 
        << "(memory test) print memory manager info" << endl;
}


