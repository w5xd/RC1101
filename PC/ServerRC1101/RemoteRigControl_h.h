

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 03:14:07 2038
 */
/* Compiler settings for import\RemoteRigControl.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __RemoteRigControl_h_h__
#define __RemoteRigControl_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IWlControlSite_FWD_DEFINED__
#define __IWlControlSite_FWD_DEFINED__
typedef interface IWlControlSite IWlControlSite;

#endif 	/* __IWlControlSite_FWD_DEFINED__ */


#ifndef __IWlRemoteRig_FWD_DEFINED__
#define __IWlRemoteRig_FWD_DEFINED__
typedef interface IWlRemoteRig IWlRemoteRig;

#endif 	/* __IWlRemoteRig_FWD_DEFINED__ */


#ifndef __IWlRemoteRigCommand_FWD_DEFINED__
#define __IWlRemoteRigCommand_FWD_DEFINED__
typedef interface IWlRemoteRigCommand IWlRemoteRigCommand;

#endif 	/* __IWlRemoteRigCommand_FWD_DEFINED__ */


#ifndef __IWlRemoteResult_FWD_DEFINED__
#define __IWlRemoteResult_FWD_DEFINED__
typedef interface IWlRemoteResult IWlRemoteResult;

#endif 	/* __IWlRemoteResult_FWD_DEFINED__ */


#ifndef __IGetWlRemoteRig_FWD_DEFINED__
#define __IGetWlRemoteRig_FWD_DEFINED__
typedef interface IGetWlRemoteRig IGetWlRemoteRig;

#endif 	/* __IGetWlRemoteRig_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


/* interface __MIDL_itf_RemoteRigControl_0000_0000 */
/* [local] */ 

#if defined(__cplusplus)
namespace WriteLog_Control_Site {
/*Default button and encoder assignments*/
static const char TRELLIS_BUTTON_SSB = 0;
static const char TRELLIS_BUTTON_CW = 1;
static const char TRELLIS_BUTTON_RTTY = 2;
static const char TRELLIS_BUTTON_SPLIT = 3;
static const char RX_ENCODER = 0;
static const char TX_ENCODER = 1;
static const USHORT GENIE_OBJ_AFGAIN_KNOB = 30000; /* out of range of genie arduino library*/
static const char AFGAIN_ENCODER = 3; /* ...unless it otherwise gets mapped*/
static const char MAIN_FORM_IDX = 0;
static const char MAIN_LED_DIGITS0_IDX = 0;
static const char MAIN_LED_DIGITS1_IDX = 1;
static const char SUB_LED_DIGITS0_IDX = 2;
static const char SUB_LED_DIGITS1_IDX = 3;
static const char MAX_SETDISPLAYOBJECTS_COUNT = 5;
}
#endif


extern RPC_IF_HANDLE __MIDL_itf_RemoteRigControl_0000_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_RemoteRigControl_0000_0000_v0_0_s_ifspec;

#ifndef __IWlControlSite_INTERFACE_DEFINED__
#define __IWlControlSite_INTERFACE_DEFINED__

/* interface IWlControlSite */
/* [unique][nonextensible][uuid][object] */ 


EXTERN_C const IID IID_IWlControlSite;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F5DFC0A0-99B8-4B81-A555-B05A5126E9A0")
    IWlControlSite : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ContinueUpdating( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetEncoderCenter( 
            /* [in] */ USHORT which,
            /* [in] */ LONG center) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE PressTrellisButton( 
            /* [in] */ USHORT which,
            /* [in] */ USHORT value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetDisplayString( 
            /* [in] */ USHORT which,
            /* [in] */ BSTR value) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetupTrellisGroup( 
            /* [in] */ USHORT which,
            /* [in] */ USHORT mask,
            /* [in] */ USHORT objType,
            /* [in] */ USHORT objIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetEncoderMap( 
            /* [in] */ USHORT which,
            /* [in] */ USHORT objTypeL,
            /* [in] */ USHORT objIndexL,
            /* [in] */ USHORT objTypeH,
            /* [in] */ USHORT objIndexH,
            /* [in] */ SHORT multiplier,
            /* [in] */ LONG lowLimit,
            /* [in] */ LONG highLimit) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetDisplayObjects( 
            /* [in] */ USHORT count,
            /* [size_is][in] */ USHORT objTypes[  ],
            /* [size_is][in] */ USHORT objIndices[  ],
            /* [size_is][in] */ USHORT values[  ]) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ResetDisplayDefaults( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetEncoderSwitchState( 
            /* [in] */ USHORT mask) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IWlControlSiteVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWlControlSite * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWlControlSite * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWlControlSite * This);
        
        HRESULT ( STDMETHODCALLTYPE *ContinueUpdating )( 
            IWlControlSite * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetEncoderCenter )( 
            IWlControlSite * This,
            /* [in] */ USHORT which,
            /* [in] */ LONG center);
        
        HRESULT ( STDMETHODCALLTYPE *PressTrellisButton )( 
            IWlControlSite * This,
            /* [in] */ USHORT which,
            /* [in] */ USHORT value);
        
        HRESULT ( STDMETHODCALLTYPE *SetDisplayString )( 
            IWlControlSite * This,
            /* [in] */ USHORT which,
            /* [in] */ BSTR value);
        
        HRESULT ( STDMETHODCALLTYPE *SetupTrellisGroup )( 
            IWlControlSite * This,
            /* [in] */ USHORT which,
            /* [in] */ USHORT mask,
            /* [in] */ USHORT objType,
            /* [in] */ USHORT objIndex);
        
        HRESULT ( STDMETHODCALLTYPE *SetEncoderMap )( 
            IWlControlSite * This,
            /* [in] */ USHORT which,
            /* [in] */ USHORT objTypeL,
            /* [in] */ USHORT objIndexL,
            /* [in] */ USHORT objTypeH,
            /* [in] */ USHORT objIndexH,
            /* [in] */ SHORT multiplier,
            /* [in] */ LONG lowLimit,
            /* [in] */ LONG highLimit);
        
        HRESULT ( STDMETHODCALLTYPE *SetDisplayObjects )( 
            IWlControlSite * This,
            /* [in] */ USHORT count,
            /* [size_is][in] */ USHORT objTypes[  ],
            /* [size_is][in] */ USHORT objIndices[  ],
            /* [size_is][in] */ USHORT values[  ]);
        
        HRESULT ( STDMETHODCALLTYPE *ResetDisplayDefaults )( 
            IWlControlSite * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetEncoderSwitchState )( 
            IWlControlSite * This,
            /* [in] */ USHORT mask);
        
        END_INTERFACE
    } IWlControlSiteVtbl;

    interface IWlControlSite
    {
        CONST_VTBL struct IWlControlSiteVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWlControlSite_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWlControlSite_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWlControlSite_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWlControlSite_ContinueUpdating(This)	\
    ( (This)->lpVtbl -> ContinueUpdating(This) ) 

#define IWlControlSite_SetEncoderCenter(This,which,center)	\
    ( (This)->lpVtbl -> SetEncoderCenter(This,which,center) ) 

#define IWlControlSite_PressTrellisButton(This,which,value)	\
    ( (This)->lpVtbl -> PressTrellisButton(This,which,value) ) 

#define IWlControlSite_SetDisplayString(This,which,value)	\
    ( (This)->lpVtbl -> SetDisplayString(This,which,value) ) 

#define IWlControlSite_SetupTrellisGroup(This,which,mask,objType,objIndex)	\
    ( (This)->lpVtbl -> SetupTrellisGroup(This,which,mask,objType,objIndex) ) 

#define IWlControlSite_SetEncoderMap(This,which,objTypeL,objIndexL,objTypeH,objIndexH,multiplier,lowLimit,highLimit)	\
    ( (This)->lpVtbl -> SetEncoderMap(This,which,objTypeL,objIndexL,objTypeH,objIndexH,multiplier,lowLimit,highLimit) ) 

#define IWlControlSite_SetDisplayObjects(This,count,objTypes,objIndices,values)	\
    ( (This)->lpVtbl -> SetDisplayObjects(This,count,objTypes,objIndices,values) ) 

#define IWlControlSite_ResetDisplayDefaults(This)	\
    ( (This)->lpVtbl -> ResetDisplayDefaults(This) ) 

#define IWlControlSite_SetEncoderSwitchState(This,mask)	\
    ( (This)->lpVtbl -> SetEncoderSwitchState(This,mask) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWlControlSite_INTERFACE_DEFINED__ */


#ifndef __IWlRemoteRig_INTERFACE_DEFINED__
#define __IWlRemoteRig_INTERFACE_DEFINED__

/* interface IWlRemoteRig */
/* [unique][nonextensible][uuid][object] */ 


EXTERN_C const IID IID_IWlRemoteRig;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F5DFC0A1-99B8-4B81-A555-B05A5126E9A0")
    IWlRemoteRig : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE RequestInitializeControls( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ControlsChanged( 
            /* [in] */ USHORT encoderCount,
            /* [size_is][in] */ LONG pEncoders[  ],
            /* [in] */ ULONG trellisMask,
            /* [in] */ USHORT encoderSwitchMask) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EndRemoteControl( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IWlRemoteRigVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWlRemoteRig * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWlRemoteRig * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWlRemoteRig * This);
        
        HRESULT ( STDMETHODCALLTYPE *RequestInitializeControls )( 
            IWlRemoteRig * This);
        
        HRESULT ( STDMETHODCALLTYPE *ControlsChanged )( 
            IWlRemoteRig * This,
            /* [in] */ USHORT encoderCount,
            /* [size_is][in] */ LONG pEncoders[  ],
            /* [in] */ ULONG trellisMask,
            /* [in] */ USHORT encoderSwitchMask);
        
        HRESULT ( STDMETHODCALLTYPE *EndRemoteControl )( 
            IWlRemoteRig * This);
        
        END_INTERFACE
    } IWlRemoteRigVtbl;

    interface IWlRemoteRig
    {
        CONST_VTBL struct IWlRemoteRigVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWlRemoteRig_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWlRemoteRig_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWlRemoteRig_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWlRemoteRig_RequestInitializeControls(This)	\
    ( (This)->lpVtbl -> RequestInitializeControls(This) ) 

#define IWlRemoteRig_ControlsChanged(This,encoderCount,pEncoders,trellisMask,encoderSwitchMask)	\
    ( (This)->lpVtbl -> ControlsChanged(This,encoderCount,pEncoders,trellisMask,encoderSwitchMask) ) 

#define IWlRemoteRig_EndRemoteControl(This)	\
    ( (This)->lpVtbl -> EndRemoteControl(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWlRemoteRig_INTERFACE_DEFINED__ */


#ifndef __IWlRemoteRigCommand_INTERFACE_DEFINED__
#define __IWlRemoteRigCommand_INTERFACE_DEFINED__

/* interface IWlRemoteRigCommand */
/* [unique][nonextensible][uuid][object] */ 


EXTERN_C const IID IID_IWlRemoteRigCommand;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F5DFC0A3-99B8-4B81-A555-B05A5126E9A0")
    IWlRemoteRigCommand : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetRigType( 
            /* [retval][out] */ BSTR *rt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetDriverParameters( 
            /* [in] */ BSTR param) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RigCommand( 
            /* [in] */ ULONG data,
            /* [in] */ USHORT count,
            /* [size_is][in] */ BYTE pBytes[  ]) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EndRemoteControl( void) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IWlRemoteRigCommandVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWlRemoteRigCommand * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWlRemoteRigCommand * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWlRemoteRigCommand * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetRigType )( 
            IWlRemoteRigCommand * This,
            /* [retval][out] */ BSTR *rt);
        
        HRESULT ( STDMETHODCALLTYPE *SetDriverParameters )( 
            IWlRemoteRigCommand * This,
            /* [in] */ BSTR param);
        
        HRESULT ( STDMETHODCALLTYPE *RigCommand )( 
            IWlRemoteRigCommand * This,
            /* [in] */ ULONG data,
            /* [in] */ USHORT count,
            /* [size_is][in] */ BYTE pBytes[  ]);
        
        HRESULT ( STDMETHODCALLTYPE *EndRemoteControl )( 
            IWlRemoteRigCommand * This);
        
        END_INTERFACE
    } IWlRemoteRigCommandVtbl;

    interface IWlRemoteRigCommand
    {
        CONST_VTBL struct IWlRemoteRigCommandVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWlRemoteRigCommand_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWlRemoteRigCommand_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWlRemoteRigCommand_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWlRemoteRigCommand_GetRigType(This,rt)	\
    ( (This)->lpVtbl -> GetRigType(This,rt) ) 

#define IWlRemoteRigCommand_SetDriverParameters(This,param)	\
    ( (This)->lpVtbl -> SetDriverParameters(This,param) ) 

#define IWlRemoteRigCommand_RigCommand(This,data,count,pBytes)	\
    ( (This)->lpVtbl -> RigCommand(This,data,count,pBytes) ) 

#define IWlRemoteRigCommand_EndRemoteControl(This)	\
    ( (This)->lpVtbl -> EndRemoteControl(This) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWlRemoteRigCommand_INTERFACE_DEFINED__ */


#ifndef __IWlRemoteResult_INTERFACE_DEFINED__
#define __IWlRemoteResult_INTERFACE_DEFINED__

/* interface IWlRemoteResult */
/* [unique][nonextensible][uuid][object] */ 


EXTERN_C const IID IID_IWlRemoteResult;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F5DFC0A4-99B8-4B81-A555-B05A5126E9A0")
    IWlRemoteResult : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE RigCommandResult( 
            /* [in] */ ULONG data,
            /* [in] */ USHORT count,
            /* [size_is][in] */ BYTE pBytes[  ]) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IWlRemoteResultVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWlRemoteResult * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWlRemoteResult * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWlRemoteResult * This);
        
        HRESULT ( STDMETHODCALLTYPE *RigCommandResult )( 
            IWlRemoteResult * This,
            /* [in] */ ULONG data,
            /* [in] */ USHORT count,
            /* [size_is][in] */ BYTE pBytes[  ]);
        
        END_INTERFACE
    } IWlRemoteResultVtbl;

    interface IWlRemoteResult
    {
        CONST_VTBL struct IWlRemoteResultVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWlRemoteResult_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWlRemoteResult_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWlRemoteResult_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWlRemoteResult_RigCommandResult(This,data,count,pBytes)	\
    ( (This)->lpVtbl -> RigCommandResult(This,data,count,pBytes) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWlRemoteResult_INTERFACE_DEFINED__ */


#ifndef __IGetWlRemoteRig_INTERFACE_DEFINED__
#define __IGetWlRemoteRig_INTERFACE_DEFINED__

/* interface IGetWlRemoteRig */
/* [unique][nonextensible][uuid][object] */ 


EXTERN_C const IID IID_IGetWlRemoteRig;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F5DFC0A2-99B8-4B81-A555-B05A5126E9A0")
    IGetWlRemoteRig : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetRemoteRig( 
            /* [in] */ REFIID iidCs,
            /* [iid_is][in] */ IUnknown *pControlSite,
            /* [in] */ REFIID iidRR,
            /* [retval][iid_is][out] */ IUnknown **ppRemote) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IGetWlRemoteRigVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IGetWlRemoteRig * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IGetWlRemoteRig * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IGetWlRemoteRig * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetRemoteRig )( 
            IGetWlRemoteRig * This,
            /* [in] */ REFIID iidCs,
            /* [iid_is][in] */ IUnknown *pControlSite,
            /* [in] */ REFIID iidRR,
            /* [retval][iid_is][out] */ IUnknown **ppRemote);
        
        END_INTERFACE
    } IGetWlRemoteRigVtbl;

    interface IGetWlRemoteRig
    {
        CONST_VTBL struct IGetWlRemoteRigVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IGetWlRemoteRig_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IGetWlRemoteRig_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IGetWlRemoteRig_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IGetWlRemoteRig_GetRemoteRig(This,iidCs,pControlSite,iidRR,ppRemote)	\
    ( (This)->lpVtbl -> GetRemoteRig(This,iidCs,pControlSite,iidRR,ppRemote) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IGetWlRemoteRig_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  BSTR_UserSize64(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal64(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal64(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree64(     unsigned long *, BSTR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


