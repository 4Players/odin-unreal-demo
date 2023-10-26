// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AdvancedSessions/Classes/StartSessionCallbackProxyAdvanced.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeStartSessionCallbackProxyAdvanced() {}
// Cross Module References
	ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UStartSessionCallbackProxyAdvanced();
	ADVANCEDSESSIONS_API UClass* Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_NoRegister();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UOnlineBlueprintCallProxyBase();
	ENGINE_API UFunction* Z_Construct_UDelegateFunction_Engine_EmptyOnlineDelegate__DelegateSignature();
	UPackage* Z_Construct_UPackage__Script_AdvancedSessions();
// End Cross Module References
	DEFINE_FUNCTION(UStartSessionCallbackProxyAdvanced::execStartAdvancedSession)
	{
		P_GET_OBJECT(UObject,Z_Param_WorldContextObject);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(UStartSessionCallbackProxyAdvanced**)Z_Param__Result=UStartSessionCallbackProxyAdvanced::StartAdvancedSession(Z_Param_WorldContextObject);
		P_NATIVE_END;
	}
	void UStartSessionCallbackProxyAdvanced::StaticRegisterNativesUStartSessionCallbackProxyAdvanced()
	{
		UClass* Class = UStartSessionCallbackProxyAdvanced::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "StartAdvancedSession", &UStartSessionCallbackProxyAdvanced::execStartAdvancedSession },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UStartSessionCallbackProxyAdvanced_StartAdvancedSession_Statics
	{
		struct StartSessionCallbackProxyAdvanced_eventStartAdvancedSession_Parms
		{
			const UObject* WorldContextObject;
			UStartSessionCallbackProxyAdvanced* ReturnValue;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WorldContextObject_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_WorldContextObject;
		static const UECodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UStartSessionCallbackProxyAdvanced_StartAdvancedSession_Statics::NewProp_WorldContextObject_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UStartSessionCallbackProxyAdvanced_StartAdvancedSession_Statics::NewProp_WorldContextObject = { "WorldContextObject", nullptr, (EPropertyFlags)0x0010000000000082, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(StartSessionCallbackProxyAdvanced_eventStartAdvancedSession_Parms, WorldContextObject), Z_Construct_UClass_UObject_NoRegister, METADATA_PARAMS(Z_Construct_UFunction_UStartSessionCallbackProxyAdvanced_StartAdvancedSession_Statics::NewProp_WorldContextObject_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UStartSessionCallbackProxyAdvanced_StartAdvancedSession_Statics::NewProp_WorldContextObject_MetaData)) };
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UStartSessionCallbackProxyAdvanced_StartAdvancedSession_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(StartSessionCallbackProxyAdvanced_eventStartAdvancedSession_Parms, ReturnValue), Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UStartSessionCallbackProxyAdvanced_StartAdvancedSession_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStartSessionCallbackProxyAdvanced_StartAdvancedSession_Statics::NewProp_WorldContextObject,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UStartSessionCallbackProxyAdvanced_StartAdvancedSession_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UStartSessionCallbackProxyAdvanced_StartAdvancedSession_Statics::Function_MetaDataParams[] = {
		{ "BlueprintInternalUseOnly", "true" },
		{ "Category", "Online|AdvancedSessions" },
		{ "Comment", "/**\n\x09 * Starts a session with the default online subsystem. The session needs to be previously created by calling the \"CreateAdvancedSession\" node.\n\x09 * @param WorldContextObject\n\x09 */" },
		{ "ModuleRelativePath", "Classes/StartSessionCallbackProxyAdvanced.h" },
		{ "ToolTip", "Starts a session with the default online subsystem. The session needs to be previously created by calling the \"CreateAdvancedSession\" node.\n@param WorldContextObject" },
		{ "WorldContext", "WorldContextObject" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UStartSessionCallbackProxyAdvanced_StartAdvancedSession_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UStartSessionCallbackProxyAdvanced, nullptr, "StartAdvancedSession", nullptr, nullptr, sizeof(Z_Construct_UFunction_UStartSessionCallbackProxyAdvanced_StartAdvancedSession_Statics::StartSessionCallbackProxyAdvanced_eventStartAdvancedSession_Parms), Z_Construct_UFunction_UStartSessionCallbackProxyAdvanced_StartAdvancedSession_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UStartSessionCallbackProxyAdvanced_StartAdvancedSession_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UStartSessionCallbackProxyAdvanced_StartAdvancedSession_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UStartSessionCallbackProxyAdvanced_StartAdvancedSession_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UStartSessionCallbackProxyAdvanced_StartAdvancedSession()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UStartSessionCallbackProxyAdvanced_StartAdvancedSession_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UStartSessionCallbackProxyAdvanced);
	UClass* Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_NoRegister()
	{
		return UStartSessionCallbackProxyAdvanced::StaticClass();
	}
	struct Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_OnSuccess_MetaData[];
#endif
		static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnSuccess;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_OnFailure_MetaData[];
#endif
		static const UECodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnFailure;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UOnlineBlueprintCallProxyBase,
		(UObject* (*)())Z_Construct_UPackage__Script_AdvancedSessions,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UStartSessionCallbackProxyAdvanced_StartAdvancedSession, "StartAdvancedSession" }, // 1830694180
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "StartSessionCallbackProxyAdvanced.h" },
		{ "ModuleRelativePath", "Classes/StartSessionCallbackProxyAdvanced.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_Statics::NewProp_OnSuccess_MetaData[] = {
		{ "Comment", "// Called when the session starts successfully\n" },
		{ "ModuleRelativePath", "Classes/StartSessionCallbackProxyAdvanced.h" },
		{ "ToolTip", "Called when the session starts successfully" },
	};
#endif
	const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_Statics::NewProp_OnSuccess = { "OnSuccess", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UStartSessionCallbackProxyAdvanced, OnSuccess), Z_Construct_UDelegateFunction_Engine_EmptyOnlineDelegate__DelegateSignature, METADATA_PARAMS(Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_Statics::NewProp_OnSuccess_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_Statics::NewProp_OnSuccess_MetaData)) }; // 2258189322
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_Statics::NewProp_OnFailure_MetaData[] = {
		{ "Comment", "// Called when there is an error starting the session\n" },
		{ "ModuleRelativePath", "Classes/StartSessionCallbackProxyAdvanced.h" },
		{ "ToolTip", "Called when there is an error starting the session" },
	};
#endif
	const UECodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_Statics::NewProp_OnFailure = { "OnFailure", nullptr, (EPropertyFlags)0x0010000010080000, UECodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UStartSessionCallbackProxyAdvanced, OnFailure), Z_Construct_UDelegateFunction_Engine_EmptyOnlineDelegate__DelegateSignature, METADATA_PARAMS(Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_Statics::NewProp_OnFailure_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_Statics::NewProp_OnFailure_MetaData)) }; // 2258189322
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_Statics::NewProp_OnSuccess,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_Statics::NewProp_OnFailure,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UStartSessionCallbackProxyAdvanced>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_Statics::ClassParams = {
		&UStartSessionCallbackProxyAdvanced::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_Statics::PropPointers),
		0,
		0x008800A0u,
		METADATA_PARAMS(Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UStartSessionCallbackProxyAdvanced()
	{
		if (!Z_Registration_Info_UClass_UStartSessionCallbackProxyAdvanced.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UStartSessionCallbackProxyAdvanced.OuterSingleton, Z_Construct_UClass_UStartSessionCallbackProxyAdvanced_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UStartSessionCallbackProxyAdvanced.OuterSingleton;
	}
	template<> ADVANCEDSESSIONS_API UClass* StaticClass<UStartSessionCallbackProxyAdvanced>()
	{
		return UStartSessionCallbackProxyAdvanced::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UStartSessionCallbackProxyAdvanced);
	UStartSessionCallbackProxyAdvanced::~UStartSessionCallbackProxyAdvanced() {}
	struct Z_CompiledInDeferFile_FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_StartSessionCallbackProxyAdvanced_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_StartSessionCallbackProxyAdvanced_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UStartSessionCallbackProxyAdvanced, UStartSessionCallbackProxyAdvanced::StaticClass, TEXT("UStartSessionCallbackProxyAdvanced"), &Z_Registration_Info_UClass_UStartSessionCallbackProxyAdvanced, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UStartSessionCallbackProxyAdvanced), 573907015U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_StartSessionCallbackProxyAdvanced_h_413441792(TEXT("/Script/AdvancedSessions"),
		Z_CompiledInDeferFile_FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_StartSessionCallbackProxyAdvanced_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_StartSessionCallbackProxyAdvanced_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
