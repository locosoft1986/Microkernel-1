#ifndef NativeSystemCalls_H
#define NativeSystemCalls_H

#include <SystemCalls/SystemCalls.h>

#define TotalSystemCalls	19

namespace SystemCalls
{
    namespace Native
    {
        namespace UserLevelStructures
        {
            struct Message
            {
                virtAddress Data;
                unsigned int Length;
                unsigned int Code;
                virtAddress Source;
                unsigned int MessageChain;
            } __attribute__((packed));

            struct MemoryBlock
            {
                unsigned int Start;
                unsigned int End;
                unsigned int CopyTo;
            } __attribute__((packed));

            struct DriverInfoBlock
            {
                char Name[32];
                bool SupportsInterrupts;
                unsigned short IRQ;
                unsigned int Type;
            } __attribute__((packed));

            struct SharedPage
            {
            } __attribute__((packed));

            //This namespace contains the structure definitions used by the RequestProcessData system call
            namespace ProcessData
            {
                struct Thread
                {
                    unsigned int TID;
                    unsigned int EIP;
                    unsigned int Status;
                } __attribute__((packed));

                struct MemoryUsageStructure
                {
                    uint64 MemoryUsed;
                    unsigned int SharedPageCount;
                } __attribute__((packed));

                struct Process
                {
                    char Name[32];
                    unsigned int PID;
                    DriverInfoBlock *Driver;
                    unsigned int State;
                    MemoryUsageStructure *MemoryUsage;
                    unsigned int ThreadCount;
                    Thread *Threads;
                } __attribute__((packed));

                struct ReturnValue
                {
                    //This is the process that the VFS runs as. All FS-related messages must be sent here
                    unsigned int VFSPointer;
                    unsigned int CurrentPID;
                    unsigned int ProcessCount;
                    Process *Processes;
                } __attribute__((packed));
            }
        }

        class Internal
        {
        private:
            Internal();
            ~Internal();
            static unsigned int isAddressValid(virtAddress address, bool checkUserMode);
        public:
            SystemCallMethod(CreateThread);
            SystemCallMethod(KillThread);
            SystemCallMethod(CreateProcess);
            SystemCallMethod(KillProcess);
            SystemCallMethod(SendMessage);
            SystemCallMethod(CreateSharedPage);
            SystemCallMethod(DestroySharedPage);
            SystemCallMethod(RequestCPUBootStub);
            SystemCallMethod(RequestIdentityMapping);
            SystemCallMethod(InstallDriver);
            SystemCallMethod(Yield);
            SystemCallMethod(PerformPageManagement);
            SystemCallMethod(SetReceiptMethod);
            SystemCallMethod(ToggleSecurityPrivilege);
            SystemCallMethod(SecurityPrivilegeStatus);
            SystemCallMethod(SleepThread);
            SystemCallMethod(WakeThread);
            SystemCallMethod(RequestProcessData);
            SystemCallMethod(GetCurrentThread);
        };

        class SystemCallInterrupt : public InterruptSink
        {
        private:
            SystemCall calls[TotalSystemCalls];
            void receive(StackStates::Interrupt *stack);
        public:
            SystemCallInterrupt();
            ~SystemCallInterrupt();
        };
    }
}

#endif
