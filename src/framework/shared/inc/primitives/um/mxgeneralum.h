/*++

Copyright (c) Microsoft Corporation

ModuleName:

    MxGeneralUm.h

Abstract:

    User mode implementation for general OS
    functions defined in MxGeneral.h

Author:



Revision History:



--*/

#pragma once

#define MAKE_MX_FUNC_NAME(x)    x

typedef VOID THREADPOOL_WAIT_CALLBACK (
    __inout     PTP_CALLBACK_INSTANCE Instance,
    __inout_opt PVOID                 Context,
    __inout     PTP_WAIT              Wait,
    __in        TP_WAIT_RESULT        WaitResult
    );

typedef THREADPOOL_WAIT_CALLBACK MdInterruptServiceRoutineType, *MdInterruptServiceRoutine;

typedef
BOOLEAN
InterruptSynchronizeRoutine (
    __in PVOID SynchronizeContext
    );

typedef InterruptSynchronizeRoutine MdInterruptSynchronizeRoutineType, *MdInterruptSynchronizeRoutine;

typedef struct _CALLBACK_OBJECT *PCALLBACK_OBJECT;

typedef 
VOID
CALLBACK_FUNCTION(
        __in PVOID CallbackContext,
        __in_opt PVOID Argument1,
        __in_opt PVOID Argument2
        );

typedef CALLBACK_FUNCTION       MdCallbackFunctionType, *MdCallbackFunction;

//
// Define PnP notification event categories
//

typedef enum _IO_NOTIFICATION_EVENT_CATEGORY {
    EventCategoryReserved,
    EventCategoryHardwareProfileChange,
    EventCategoryDeviceInterfaceChange,
    EventCategoryTargetDeviceChange
} IO_NOTIFICATION_EVENT_CATEGORY;

#include "MxGeneral.h"

FORCEINLINE
BOOLEAN
Mx::IsUM(
    )
{
    return TRUE;
}

FORCEINLINE
BOOLEAN
Mx::IsKM(
    )
{
    return FALSE;
}

FORCEINLINE
MxThread
Mx::MxGetCurrentThread(
    )
{
    //
    // We can't use GetCurrentThread as it returns a pseudo handle
    // which would have same numeric value for different threads
    // We could use DuplicateHandle to get real handle but that has the 
    // following problems:
    //    1) It returns different handle values for the same thread
    //       if called again without closing handle.
    //    2) Makes the caller call CloseHandle making it inconvenient to
    //       call this function just to get an identifier for the thread
    //    3) More expensive than GetCurrentThreadId
    //
    // Since framework uses the thread only for comparison, logging
    // purposes GetCurrentThreadId works well.
    // It is cast to PVOID to match the pointer type PKTHREAD otherwise
    // trace functions complain of data type mismatch
    //

    return (PVOID) ::GetCurrentThreadId();
}

FORCEINLINE
MdEThread
Mx::GetCurrentEThread(
    )
{
    //
    // See comments in MxGetCurrentThread.
    //
    return (PVOID) MxGetCurrentThread();
}

FORCEINLINE
NTSTATUS
Mx::MxTerminateCurrentThread(
    __in NTSTATUS Status
    )
{
    #pragma prefast(suppress:__WARNING_USINGTERMINATETHREAD, "TerminateThread is the intent.");
    if (!TerminateThread(::GetCurrentThread(), HRESULT_FROM_NT(Status))) {
        DWORD err = GetLastError();
        return WinErrorToNtStatus(err);
    }
    return STATUS_SUCCESS;
}

FORCEINLINE
KIRQL
Mx::MxGetCurrentIrql(
    )
{
    return PASSIVE_LEVEL;
}

FORCEINLINE
VOID
#pragma prefast(suppress:__WARNING_UNMATCHED_DECL_ANNO, "Can't apply kernel mode annotations.");
Mx::MxRaiseIrql(
    __in KIRQL  NewIrql,
    __out PKIRQL  OldIrql
    )
{
    UNREFERENCED_PARAMETER(NewIrql);
    UNREFERENCED_PARAMETER(OldIrql);

    DO_NOTHING();
}

FORCEINLINE
VOID
#pragma prefast(suppress:__WARNING_UNMATCHED_DECL_ANNO, "Can't apply kernel mode annotations.");
Mx::MxLowerIrql(
    __in KIRQL  NewIrql
    )
{
    UNREFERENCED_PARAMETER(NewIrql);

    DO_NOTHING();
}

FORCEINLINE
VOID
Mx::MxQueryTickCount(
    __out PLARGE_INTEGER  TickCount
    )
{
    TickCount->QuadPart = GetTickCount();
}

FORCEINLINE
VOID
Mx::MxDbgBreakPoint(
    )
{
    DebugBreak();
}

FORCEINLINE
VOID
Mx::MxAssert(
    __in BOOLEAN Condition
    )
{
    if (!Condition)
    {



        DebugBreak();
    }
}

FORCEINLINE
VOID
Mx::MxAssertMsg(
    __in LPSTR Message,
    __in BOOLEAN Condition
    )
{
    UNREFERENCED_PARAMETER(Message);

    if (!Condition)
    {



        DebugBreak();
    }
}

FORCEINLINE
VOID
#pragma prefast(suppress:__WARNING_UNMATCHED_DEFN, "Can't apply kernel mode annotations.");
Mx::MxEnterCriticalRegion(
    )
{





    // DO_NOTHING();
}

FORCEINLINE
VOID
#pragma prefast(suppress:__WARNING_UNMATCHED_DEFN, "Can't apply kernel mode annotations.");
Mx::MxLeaveCriticalRegion(
    )
{





    // DO_NOTHING();
}

FORCEINLINE
VOID
Mx::MxDelayExecutionThread(
    __in KPROCESSOR_MODE  WaitMode,
    __in BOOLEAN  Alertable,
    __in PLARGE_INTEGER  Interval
    )
{
    UNREFERENCED_PARAMETER(WaitMode);

    LARGE_INTEGER intervalMillisecond;

    if (Interval < 0)
    {
        intervalMillisecond.QuadPart = -1 * Interval->QuadPart;
    }
    else
    {
        intervalMillisecond.QuadPart = Interval->QuadPart;
    }

    intervalMillisecond.QuadPart /= 10000;

    SleepEx((DWORD)intervalMillisecond.QuadPart, Alertable);
}

FORCEINLINE
PVOID
Mx::MxGetSystemRoutineAddress(
    __in MxFuncName FuncName
    )
/*++
Description:

    This function is meant to be called only by mode agnostic code
    System routine is assumed to be in ntdll.dll.

    This is because system routines (Rtl*) that can be used both
    in kernel mode as well as user mode reside in ntdll.dll.
    Kernel32.dll contains the user mode only Win32 API.

Arguments:

    MxFuncName FuncName -

Return Value:

    NTSTATUS Status code.
--*/
{
    HMODULE hMod;

    hMod = GetModuleHandleW(L"ntdll.dll");

    return GetProcAddress(hMod, FuncName);
}

FORCEINLINE
VOID
Mx::MxReferenceObject(
    __in PVOID Object
    )
{
    UNREFERENCED_PARAMETER(Object);






    // DO_NOTHING();
}

FORCEINLINE
VOID
Mx::MxDereferenceObject(
    __in PVOID Object
    )
{
    UNREFERENCED_PARAMETER(Object);






    // DO_NOTHING();
}

FORCEINLINE
VOID
Mx::MxInitializeRemoveLock(
    __in MdRemoveLock  Lock,
    __in ULONG  AllocateTag,
    __in ULONG  MaxLockedMinutes,
    __in ULONG  HighWatermark
    )
{
    UNREFERENCED_PARAMETER(Lock);
    UNREFERENCED_PARAMETER(AllocateTag);
    UNREFERENCED_PARAMETER(MaxLockedMinutes);
    UNREFERENCED_PARAMETER(HighWatermark);




}

FORCEINLINE
NTSTATUS
Mx::MxAcquireRemoveLock(
    __in MdRemoveLock  RemoveLock,
    __in_opt PVOID  Tag 
    )
{
    UNREFERENCED_PARAMETER(RemoveLock);
    UNREFERENCED_PARAMETER(Tag);





    return STATUS_SUCCESS;
}

FORCEINLINE
VOID
Mx::MxReleaseRemoveLock(
    __in MdRemoveLock  RemoveLock,
    __in PVOID  Tag 
    )
{
    UNREFERENCED_PARAMETER(RemoveLock);
    UNREFERENCED_PARAMETER(Tag);




}

FORCEINLINE
VOID
Mx::MxReleaseRemoveLockAndWait(
    __in MdRemoveLock  RemoveLock,
    __in PVOID  Tag 
    )
{
    UNREFERENCED_PARAMETER(RemoveLock);
    UNREFERENCED_PARAMETER(Tag);




}

FORCEINLINE
BOOLEAN
Mx::MxHasEnoughRemainingThreadStack(
    VOID
    )
{





    // Thread stack is not so scarce in UM so return TRUE always
    //
    return TRUE;
}

FORCEINLINE
VOID
#pragma prefast(suppress:__WARNING_UNMATCHED_DECL_ANNO, "Can't apply kernel mode annotations.");
Mx::ReleaseCancelSpinLock(
    __in KIRQL  Irql
    )
{
    UNREFERENCED_PARAMETER(Irql);

    //
    // UMDF Host doesn't have cancel spinlock equivalent concept so do nothing.
    //
    DO_NOTHING();
}

FORCEINLINE
NTSTATUS
Mx::CreateCallback(
    __out PCALLBACK_OBJECT  *CallbackObject,
    __in POBJECT_ATTRIBUTES  ObjectAttributes,
    __in BOOLEAN  Create,
    __in BOOLEAN  AllowMultipleCallbacks
    )
{
    UNREFERENCED_PARAMETER(CallbackObject);
    UNREFERENCED_PARAMETER(ObjectAttributes);
    UNREFERENCED_PARAMETER(Create);
    UNREFERENCED_PARAMETER(AllowMultipleCallbacks);

    return STATUS_UNSUCCESSFUL;
}

FORCEINLINE
PVOID
Mx::RegisterCallback(
    __in PCALLBACK_OBJECT  CallbackObject,
    __in MdCallbackFunction  CallbackFunction,
    __in PVOID  CallbackContext
    )
{
    UNREFERENCED_PARAMETER(CallbackObject);
    UNREFERENCED_PARAMETER(CallbackFunction);
    UNREFERENCED_PARAMETER(CallbackContext);

    ASSERTMSG("Not implemented for UMDF\n", FALSE);

    return NULL;
}

FORCEINLINE
VOID
Mx::UnregisterCallback(
    __in PVOID  CbRegistration
    )
{
    UNREFERENCED_PARAMETER(CbRegistration);

    ASSERTMSG("Not implemented for UMDF\n", FALSE);
}

FORCEINLINE
VOID
Mx::MxUnlockPages(
    __in PMDL Mdl
    )
{
    UNREFERENCED_PARAMETER(Mdl);

    ASSERTMSG("Not implemented for UMDF\n", FALSE);
}

FORCEINLINE
PVOID
Mx::MxGetSystemAddressForMdlSafe(
    __inout PMDL Mdl,
    __in    ULONG Priority
    )
{
    UNREFERENCED_PARAMETER(Mdl);
    UNREFERENCED_PARAMETER(Priority);

    ASSERTMSG("Not implemented for UMDF\n", FALSE);

    return NULL;
}

FORCEINLINE
VOID
Mx::MxBuildMdlForNonPagedPool(
    __inout PMDL Mdl
    )
{
    UNREFERENCED_PARAMETER(Mdl);

    ASSERTMSG("Not implemented for UMDF\n", FALSE);
}

FORCEINLINE
PVOID
Mx::MxGetDriverObjectExtension(
    __in MdDriverObject DriverObject,
    __in PVOID ClientIdentificationAddress
    )
{
    UNREFERENCED_PARAMETER(DriverObject);
    UNREFERENCED_PARAMETER(ClientIdentificationAddress);

    ASSERTMSG("Not implemented for UMDF\n", FALSE);

    return NULL;
}

FORCEINLINE
NTSTATUS
Mx::MxAllocateDriverObjectExtension(
    _In_  MdDriverObject DriverObject,
    _In_  PVOID ClientIdentificationAddress,
    _In_  ULONG DriverObjectExtensionSize,
    // When successful, this always allocates already-aliased memory.
    _Post_ _At_(*DriverObjectExtension, _When_(return==0,
    __drv_aliasesMem __drv_allocatesMem(Mem) _Post_notnull_))
    _When_(return == 0, _Outptr_result_bytebuffer_(DriverObjectExtensionSize))
    PVOID *DriverObjectExtension
    )
{
    UNREFERENCED_PARAMETER(DriverObject);
    UNREFERENCED_PARAMETER(ClientIdentificationAddress);
    UNREFERENCED_PARAMETER(DriverObjectExtensionSize);
    UNREFERENCED_PARAMETER(DriverObjectExtension);

    ASSERTMSG("Not implemented for UMDF\n", FALSE);

    return STATUS_UNSUCCESSFUL;
}

FORCEINLINE
MdDeviceObject
Mx::MxGetAttachedDeviceReference(
    __in MdDeviceObject DriverObject
    )
{
    UNREFERENCED_PARAMETER(DriverObject);

    ASSERTMSG("Not implemented for UMDF\n", FALSE);

    return NULL;
}

FORCEINLINE
VOID
Mx::MxDeleteSymbolicLink(
    __in PUNICODE_STRING Value
    )
{
    UNREFERENCED_PARAMETER(Value);

    ASSERTMSG("Not implemented for UMDF\n", FALSE);
}

FORCEINLINE
VOID
Mx::MxDeleteNPagedLookasideList(
    _In_ PNPAGED_LOOKASIDE_LIST LookasideList
    )
{
    UNREFERENCED_PARAMETER(LookasideList);
}

FORCEINLINE
VOID
Mx::MxDeletePagedLookasideList(
    _In_ PPAGED_LOOKASIDE_LIST LookasideList
    )
{
    UNREFERENCED_PARAMETER(LookasideList);

    ASSERTMSG("Not implemented for UMDF\n", FALSE);
}

FORCEINLINE
VOID
Mx::MxInitializeNPagedLookasideList(
    _Out_     PNPAGED_LOOKASIDE_LIST Lookaside,
    _In_opt_  PALLOCATE_FUNCTION Allocate,
    _In_opt_  PFREE_FUNCTION Free,
    _In_      ULONG Flags,
    _In_      SIZE_T Size,
    _In_      ULONG Tag,
    _In_      USHORT Depth
    )
{

    UNREFERENCED_PARAMETER(Lookaside);
    UNREFERENCED_PARAMETER(Allocate);
    UNREFERENCED_PARAMETER(Free);
    UNREFERENCED_PARAMETER(Flags);
    UNREFERENCED_PARAMETER(Size);
    UNREFERENCED_PARAMETER(Tag);
    UNREFERENCED_PARAMETER(Depth);

    //ASSERTMSG("Not implemented for UMDF\n", FALSE);

}

FORCEINLINE
VOID
Mx::MxInitializePagedLookasideList(
    _Out_     PPAGED_LOOKASIDE_LIST Lookaside,
    _In_opt_  PALLOCATE_FUNCTION Allocate,
    _In_opt_  PFREE_FUNCTION Free,
    _In_      ULONG Flags,
    _In_      SIZE_T Size,
    _In_      ULONG Tag,
    _In_      USHORT Depth
    )
{

    UNREFERENCED_PARAMETER(Lookaside);
    UNREFERENCED_PARAMETER(Allocate);
    UNREFERENCED_PARAMETER(Free);
    UNREFERENCED_PARAMETER(Flags);
    UNREFERENCED_PARAMETER(Size);
    UNREFERENCED_PARAMETER(Tag);
    UNREFERENCED_PARAMETER(Depth);

    //ASSERTMSG("Not implemented for UMDF\n", FALSE);

}

FORCEINLINE
VOID
Mx::MxDeleteDevice(
    _In_ MdDeviceObject Device
    )
{
    UNREFERENCED_PARAMETER(Device);





    // Host's device stack object holds the only reference to the host devices. 
    // The infrastructure controls the device object's lifetime.
    // 
    DO_NOTHING();
}

FORCEINLINE 
NTSTATUS
Mx::MxCreateDeviceSecure(
      _In_      MdDriverObject DriverObject,
      _In_      ULONG DeviceExtensionSize,
      _In_opt_  PUNICODE_STRING DeviceName,
      _In_      DEVICE_TYPE DeviceType,
      _In_      ULONG DeviceCharacteristics,
      _In_      BOOLEAN Exclusive,
      _In_      PCUNICODE_STRING DefaultSDDLString,
      _In_opt_  LPCGUID DeviceClassGuid,
      _Out_opt_     MdDeviceObject *DeviceObject
    )
{
    UNREFERENCED_PARAMETER(DriverObject);
    UNREFERENCED_PARAMETER(DeviceExtensionSize);
    UNREFERENCED_PARAMETER(DeviceName);
    UNREFERENCED_PARAMETER(DeviceType);
    UNREFERENCED_PARAMETER(Exclusive);
    UNREFERENCED_PARAMETER(DeviceCharacteristics);
    UNREFERENCED_PARAMETER(DefaultSDDLString);
    UNREFERENCED_PARAMETER(DeviceClassGuid);
    UNREFERENCED_PARAMETER(DeviceObject);

    ASSERTMSG("Not implemented for UMDF\n", FALSE);

    return STATUS_SUCCESS;
}

FORCEINLINE
MdDeviceObject
Mx::MxAttachDeviceToDeviceStack(
    _In_ MdDeviceObject SourceDevice,
    _In_ MdDeviceObject TargetDevice
    )
{
    
    UNREFERENCED_PARAMETER(SourceDevice);
    UNREFERENCED_PARAMETER(TargetDevice);

    ASSERTMSG("Not implemented for UMDF\n", FALSE);

    return NULL;
}

FORCEINLINE
NTSTATUS 
Mx::MxCreateDevice(
    _In_      MdDriverObject DriverObject,
    _In_      ULONG DeviceExtensionSize,
    _In_opt_  PUNICODE_STRING DeviceName,
    _In_      DEVICE_TYPE DeviceType,
    _In_      ULONG DeviceCharacteristics,
    _In_      BOOLEAN Exclusive,
    _Out_opt_     MdDeviceObject *DeviceObject
    )
{
    UNREFERENCED_PARAMETER(DriverObject);
    UNREFERENCED_PARAMETER(DeviceExtensionSize);
    UNREFERENCED_PARAMETER(DeviceName);
    UNREFERENCED_PARAMETER(DeviceType);
    UNREFERENCED_PARAMETER(DeviceCharacteristics);
    UNREFERENCED_PARAMETER(Exclusive);
    UNREFERENCED_PARAMETER(DeviceObject);

    ASSERTMSG("Not implemented for UMDF\n", FALSE);

    return STATUS_SUCCESS;

}

FORCEINLINE
NTSTATUS
Mx::MxCreateSymbolicLink(
    _In_ PUNICODE_STRING SymbolicLinkName,
    _In_ PUNICODE_STRING DeviceName
    )
{
    UNREFERENCED_PARAMETER(SymbolicLinkName);
    UNREFERENCED_PARAMETER(DeviceName);

    ASSERTMSG("Not implemented for UMDF\n", FALSE);

    return STATUS_NOT_IMPLEMENTED;
}

FORCEINLINE
VOID
Mx::MxFlushQueuedDpcs(
    )
{
    //
    // Not supported for UMDF
    //
}

FORCEINLINE
NTSTATUS
Mx::MxOpenKey(
    _In_ PHANDLE KeyHandle,
    _In_ ACCESS_MASK DesiredAccess,
    _In_ POBJECT_ATTRIBUTES ObjectAttributes
    )
{
    UNREFERENCED_PARAMETER(KeyHandle);
    UNREFERENCED_PARAMETER(DesiredAccess);
    UNREFERENCED_PARAMETER(ObjectAttributes);
        
    ASSERTMSG("Not implemented for UMDF\n", FALSE);

    return STATUS_NOT_IMPLEMENTED;
}

FORCEINLINE
NTSTATUS
Mx::MxSetDeviceInterfaceState(
    _In_ PUNICODE_STRING SymbolicLinkName,
    _In_ BOOLEAN Enable
    )
{
    UNREFERENCED_PARAMETER(SymbolicLinkName);
    UNREFERENCED_PARAMETER(Enable);
        
    ASSERTMSG("Not implemented for UMDF\n", FALSE);

    return STATUS_NOT_IMPLEMENTED;
}


FORCEINLINE
NTSTATUS
Mx::MxRegisterDeviceInterface(
    _In_      PDEVICE_OBJECT PhysicalDeviceObject,
    _In_      const GUID *InterfaceClassGuid,
    _In_opt_  PUNICODE_STRING ReferenceString,
    _Out_     PUNICODE_STRING SymbolicLinkName
    )
{
    UNREFERENCED_PARAMETER(PhysicalDeviceObject);
    UNREFERENCED_PARAMETER(InterfaceClassGuid);
    UNREFERENCED_PARAMETER(ReferenceString);
    UNREFERENCED_PARAMETER(SymbolicLinkName);
    
    ASSERTMSG("Not implemented for UMDF\n", FALSE);

    return STATUS_NOT_IMPLEMENTED;
}

FORCEINLINE
NTSTATUS
Mx::MxDeleteKey(
    _In_ HANDLE KeyHandle
    )
{
    UNREFERENCED_PARAMETER(KeyHandle);
    
    ASSERTMSG("Not implemented for UMDF\n", FALSE);

    return STATUS_NOT_IMPLEMENTED;
}

FORCEINLINE
VOID 
Mx::MxInitializeMdl(
    _In_  PMDL MemoryDescriptorList,
    _In_  PVOID BaseVa,
    _In_  SIZE_T Length
    )
{
    UNREFERENCED_PARAMETER(MemoryDescriptorList);
    UNREFERENCED_PARAMETER(BaseVa);
    UNREFERENCED_PARAMETER(Length);
    
    ASSERTMSG("Not implemented for UMDF\n", FALSE);

}

FORCEINLINE
PVOID
Mx::MxGetMdlVirtualAddress(
    _In_ PMDL Mdl
    )
{
    UNREFERENCED_PARAMETER(Mdl);
    
    ASSERTMSG("Not implemented for UMDF\n", FALSE);

    return NULL;
}

FORCEINLINE
VOID 
Mx::MxBuildPartialMdl(
    _In_     PMDL SourceMdl,
    _Inout_  PMDL TargetMdl,
    _In_     PVOID VirtualAddress,
    _In_     ULONG Length
    )
{
    UNREFERENCED_PARAMETER(SourceMdl);
    UNREFERENCED_PARAMETER(TargetMdl);
    UNREFERENCED_PARAMETER(VirtualAddress);
    UNREFERENCED_PARAMETER(Length);
    
    ASSERTMSG("Not implemented for UMDF\n", FALSE);
}

FORCEINLINE
VOID 
Mx::MxQuerySystemTime(
    _Out_ PLARGE_INTEGER CurrentTime
    )
{
    UNREFERENCED_PARAMETER(CurrentTime);
    
    ASSERTMSG("Not implemented for UMDF\n", FALSE);
}

FORCEINLINE
NTSTATUS 
Mx::MxSetValueKey(
    _In_      HANDLE KeyHandle,
    _In_      PUNICODE_STRING ValueName,
    _In_opt_  ULONG TitleIndex,
    _In_      ULONG Type,
    _In_opt_  PVOID Data,
    _In_      ULONG DataSize
    )
{
    UNREFERENCED_PARAMETER(KeyHandle);
    UNREFERENCED_PARAMETER(ValueName);
    UNREFERENCED_PARAMETER(TitleIndex);
    UNREFERENCED_PARAMETER(Type);
    UNREFERENCED_PARAMETER(Data);
    UNREFERENCED_PARAMETER(DataSize);    
    
    ASSERTMSG("Not implemented for UMDF\n", FALSE);

    return STATUS_NOT_IMPLEMENTED;
}

FORCEINLINE
NTSTATUS 
Mx::MxQueryValueKey(
    _In_       HANDLE KeyHandle,
    _In_       PUNICODE_STRING ValueName,
    _In_       KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
    _Out_opt_  PVOID KeyValueInformation,
    _In_       ULONG Length,
    _Out_      PULONG ResultLength
)
{
    UNREFERENCED_PARAMETER(KeyHandle);
    UNREFERENCED_PARAMETER(ValueName);
    UNREFERENCED_PARAMETER(KeyValueInformationClass);
    UNREFERENCED_PARAMETER(KeyValueInformation);
    UNREFERENCED_PARAMETER(Length);
    UNREFERENCED_PARAMETER(ResultLength);    
    
    ASSERTMSG("Not implemented for UMDF\n", FALSE);

    return STATUS_NOT_IMPLEMENTED;
}

FORCEINLINE
NTSTATUS
Mx::MxUnRegisterPlugPlayNotification(
    __in __drv_freesMem(Pool) PVOID NotificationEntry
    )
{
    UNREFERENCED_PARAMETER(NotificationEntry);

    ASSERTMSG("Not implemented for UMDF\n", FALSE);

    return STATUS_NOT_IMPLEMENTED;
}

FORCEINLINE
NTSTATUS
Mx::MxReferenceObjectByHandle(
    __in HANDLE Handle,
    __in ACCESS_MASK DesiredAccess,
    __in_opt POBJECT_TYPE ObjectType,
    __in KPROCESSOR_MODE AccessMode,
    __out  PVOID *Object,
    __out_opt POBJECT_HANDLE_INFORMATION HandleInformation
    )
{
    UNREFERENCED_PARAMETER(Handle);
    UNREFERENCED_PARAMETER(DesiredAccess);
    UNREFERENCED_PARAMETER(ObjectType);
    UNREFERENCED_PARAMETER(AccessMode);
    UNREFERENCED_PARAMETER(Object);
    UNREFERENCED_PARAMETER(HandleInformation);

    ASSERTMSG("Not implemented for UMDF\n", FALSE);

    return STATUS_NOT_IMPLEMENTED;
}

FORCEINLINE
NTSTATUS
Mx::MxClose(
    __in HANDLE Handle
    )
{
    CloseHandle(Handle);

    return STATUS_SUCCESS;
}

FORCEINLINE
KIRQL
Mx::MxAcquireInterruptSpinLock(
    _Inout_ PKINTERRUPT Interrupt
    )
{
    UNREFERENCED_PARAMETER(Interrupt);

    ASSERTMSG("Not implemented for UMDF\n", FALSE);
    return PASSIVE_LEVEL;
}

FORCEINLINE
VOID
Mx::MxReleaseInterruptSpinLock(
    _Inout_ PKINTERRUPT Interrupt,
    _In_ KIRQL OldIrql
    )
{
    UNREFERENCED_PARAMETER(Interrupt);
    UNREFERENCED_PARAMETER(OldIrql);

    ASSERTMSG("Not implemented for UMDF\n", FALSE);
}

FORCEINLINE
BOOLEAN 
Mx::MxInsertQueueDpc(
  __inout   PRKDPC Dpc,
  __in_opt  PVOID SystemArgument1,
  __in_opt  PVOID SystemArgument2
)
{
    UNREFERENCED_PARAMETER(Dpc);
    UNREFERENCED_PARAMETER(SystemArgument1);
    UNREFERENCED_PARAMETER(SystemArgument2);

    ASSERTMSG("Not implemented for UMDF\n", FALSE);
    return FALSE;
}

