// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "FindSessionsCallbackProxyAdvanced.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class APlayerController;
class UFindSessionsCallbackProxyAdvanced;
class UObject;
enum class EBPServerPresenceSearchType : uint8;
struct FBlueprintSessionResult;
struct FSessionsSearchSetting;
#ifdef ADVANCEDSESSIONS_FindSessionsCallbackProxyAdvanced_generated_h
#error "FindSessionsCallbackProxyAdvanced.generated.h already included, missing '#pragma once' in FindSessionsCallbackProxyAdvanced.h"
#endif
#define ADVANCEDSESSIONS_FindSessionsCallbackProxyAdvanced_generated_h

#define FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_FindSessionsCallbackProxyAdvanced_h_19_SPARSE_DATA
#define FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_FindSessionsCallbackProxyAdvanced_h_19_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execFilterSessionResults); \
	DECLARE_FUNCTION(execFindSessionsAdvanced);


#define FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_FindSessionsCallbackProxyAdvanced_h_19_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execFilterSessionResults); \
	DECLARE_FUNCTION(execFindSessionsAdvanced);


#define FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_FindSessionsCallbackProxyAdvanced_h_19_ACCESSORS
#define FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_FindSessionsCallbackProxyAdvanced_h_19_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUFindSessionsCallbackProxyAdvanced(); \
	friend struct Z_Construct_UClass_UFindSessionsCallbackProxyAdvanced_Statics; \
public: \
	DECLARE_CLASS(UFindSessionsCallbackProxyAdvanced, UOnlineBlueprintCallProxyBase, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/AdvancedSessions"), ADVANCEDSESSIONS_API) \
	DECLARE_SERIALIZER(UFindSessionsCallbackProxyAdvanced)


#define FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_FindSessionsCallbackProxyAdvanced_h_19_INCLASS \
private: \
	static void StaticRegisterNativesUFindSessionsCallbackProxyAdvanced(); \
	friend struct Z_Construct_UClass_UFindSessionsCallbackProxyAdvanced_Statics; \
public: \
	DECLARE_CLASS(UFindSessionsCallbackProxyAdvanced, UOnlineBlueprintCallProxyBase, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/AdvancedSessions"), ADVANCEDSESSIONS_API) \
	DECLARE_SERIALIZER(UFindSessionsCallbackProxyAdvanced)


#define FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_FindSessionsCallbackProxyAdvanced_h_19_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	ADVANCEDSESSIONS_API UFindSessionsCallbackProxyAdvanced(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UFindSessionsCallbackProxyAdvanced) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(ADVANCEDSESSIONS_API, UFindSessionsCallbackProxyAdvanced); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UFindSessionsCallbackProxyAdvanced); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	ADVANCEDSESSIONS_API UFindSessionsCallbackProxyAdvanced(UFindSessionsCallbackProxyAdvanced&&); \
	ADVANCEDSESSIONS_API UFindSessionsCallbackProxyAdvanced(const UFindSessionsCallbackProxyAdvanced&); \
public: \
	ADVANCEDSESSIONS_API virtual ~UFindSessionsCallbackProxyAdvanced();


#define FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_FindSessionsCallbackProxyAdvanced_h_19_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	ADVANCEDSESSIONS_API UFindSessionsCallbackProxyAdvanced(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	ADVANCEDSESSIONS_API UFindSessionsCallbackProxyAdvanced(UFindSessionsCallbackProxyAdvanced&&); \
	ADVANCEDSESSIONS_API UFindSessionsCallbackProxyAdvanced(const UFindSessionsCallbackProxyAdvanced&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(ADVANCEDSESSIONS_API, UFindSessionsCallbackProxyAdvanced); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UFindSessionsCallbackProxyAdvanced); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UFindSessionsCallbackProxyAdvanced) \
	ADVANCEDSESSIONS_API virtual ~UFindSessionsCallbackProxyAdvanced();


#define FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_FindSessionsCallbackProxyAdvanced_h_16_PROLOG
#define FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_FindSessionsCallbackProxyAdvanced_h_19_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_FindSessionsCallbackProxyAdvanced_h_19_SPARSE_DATA \
	FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_FindSessionsCallbackProxyAdvanced_h_19_RPC_WRAPPERS \
	FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_FindSessionsCallbackProxyAdvanced_h_19_ACCESSORS \
	FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_FindSessionsCallbackProxyAdvanced_h_19_INCLASS \
	FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_FindSessionsCallbackProxyAdvanced_h_19_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_FindSessionsCallbackProxyAdvanced_h_19_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_FindSessionsCallbackProxyAdvanced_h_19_SPARSE_DATA \
	FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_FindSessionsCallbackProxyAdvanced_h_19_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_FindSessionsCallbackProxyAdvanced_h_19_ACCESSORS \
	FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_FindSessionsCallbackProxyAdvanced_h_19_INCLASS_NO_PURE_DECLS \
	FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_FindSessionsCallbackProxyAdvanced_h_19_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class FindSessionsCallbackProxyAdvanced."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> ADVANCEDSESSIONS_API UClass* StaticClass<class UFindSessionsCallbackProxyAdvanced>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_PluginExpProject_Plugins_AdvancedSessions_AdvancedSessions_Source_AdvancedSessions_Classes_FindSessionsCallbackProxyAdvanced_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
