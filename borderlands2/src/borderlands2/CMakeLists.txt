cmake_minimum_required(VERSION 3.14)
cmake_policy(SET CMP0087 NEW)

set(Borderlands2_Save_LIB_HEADER_DIR ${Borderlands2_Save_LIB_INCLUDE_DIR}/borderlands2)

set(Borderlands2_Save_LIB_PUBLIC_INCLUDE_FILES
        ${Borderlands2_Save_LIB_HEADER_DIR}/borderlands2.hpp
        ${CMAKE_CURRENT_BINARY_DIR}/bl2_save_lib_exports.hpp
        )

set(Borderlands2_Save_LIB_PROTO_DIR ${Borderlands2_Save_LIB_RESOURCE_DIR}/protobuf)

set(Borderlands2_Save_LIB_PROTO_FILES
        ${Borderlands2_Save_LIB_PROTO_DIR}/BankSlot.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/ChallengeData.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/ChosenVehicleCustomization.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/Color.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/DLCExpansionData.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/GoldenKeys.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/GUID.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/InventorySlotData.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/ItemData.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/ItemMemento.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/LockoutData.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/MissionData.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/MissionPlaythroughData.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/MissionStatus.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/OneOffLevelChallengeData.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/PackedItemData.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/PackedItemDataOptional.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/PackedWeaponData.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/PackedWeaponDataOptional.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/PendingMissionRewards.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/PlayerMark.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/QuickWeaponSlot.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/RegionGameStageData.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/ResourceData.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/SkillData.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/UIPreferencesData.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/WeaponData.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/WeaponMemento.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/WorldDiscoveryData.proto
        ${Borderlands2_Save_LIB_PROTO_DIR}/WillowTwoPlayerSaveGame.proto
        )

protobuf_generate_cpp(Borderlands2_Save_LIB_PROTO_SRCS
        Borderlands2_Save_LIB_PROTO_HDRS
        ${Borderlands2_Save_LIB_PROTO_FILES}
        )

set(Borderlands2_Save_LIB_PRIVATE_INCLUDE_FILES
        ${Borderlands2_Save_LIB_PROTO_HDRS}
        )

set(Borderlands2_Save_LIB_INCLUDE_FILES
        ${Borderlands2_Save_LIB_PUBLIC_INCLUDE_FILES}
        ${Borderlands2_Save_LIB_PRIVATE_INCLUDE_FILES}
        )

set(Borderlands2_Save_LIB_SOURCE_FILES
        ${Borderlands2_Save_LIB_PROTO_SRCS}
        ${CMAKE_CURRENT_SOURCE_DIR}/borderlands2.cpp
        )

set(Borderlands2_Save_LIB_RESOURCE_FILES)

if (WIN32)
    include(generate_product_version)
    generate_product_version(
            BorderlandsSaveEditor_Borderlands2_VersionFilesOutputVariable
            NAME                "Borderlands2"
            VERSION_MAJOR       ${PROJECT_VERSION_MAJOR}
            VERSION_MINOR       ${PROJECT_VERSION_MINOR}
            VERSION_PATCH       ${PROJECT_VERSION_PATCH}
            VERSION_REVISION    ${PROJECT_VERSION_TWEAK}
    )
    set(Borderlands2_Save_LIB_RESOURCE_FILES
            ${Borderlands2_Save_LIB_RESOURCE_FILES}
            ${Borderlands2_Save_LIB_VersionFilesOutputVariable}
            )
endif (WIN32)

add_library(Borderlands2_Save_LIB SHARED
        ${Borderlands2_Save_LIB_SOURCE_FILES}
        ${Borderlands2_Save_LIB_INCLUDE_FILES}
        ${Borderlands2_Save_LIB_RESOURCE_FILES}
        )

generate_export_header(Borderlands2_Save_LIB
        BASE_NAME                   Borderlands2SaveLib
        EXPORT_FILE_NAME            bl2_save_lib_exports.hpp
        EXPORT_MACRO_NAME           BORDERLANDS2_SAVE_LIB_API
        DEPRECATED_MACRO_NAME       BORDERLANDS2_SAVE_LIB_API_DEPRECATED
        NO_EXPORT_MACRO_NAME        BORDERLANDS2_SAVE_LIB_API_NO_EXPORT
        NO_DEPRECATED_MACRO_NAME    BORDERLANDS2_SAVE_LIB_API_NO_DEPRECATED
        )
target_include_directories(Borderlands2_Save_LIB
        PUBLIC ${Borderlands2_Save_LIB_INCLUDE_DIR}
        PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(Borderlands2_Save_LIB
        PUBLIC
        Borderlands_Common_LIB
        EXTERNAL_MiniLZO_LIB
        PRIVATE
        OpenSSL::Crypto
        Boost::boost
        ${Protobuf_LIBRARIES}
 #       stdc++fs
        )

target_compile_options(Borderlands2_Save_LIB
        PRIVATE -DBorderlands2_Save_LIB_EXPORTS=1
        )

set_target_properties(Borderlands2_Save_LIB
        PROPERTIES
        OUTPUT_NAME     "Borderlands2Save"
        LANGUAGES       CXX
        PUBLIC_HEADERS  "${Borderlands2_Save_LIB_PUBLIC_INCLUDE_FILES}"
        PRIVATE_HEADERS "${Borderlands2_Save_LIB_PRIVATE_INCLUDE_FILES}"
        VERSION         "${PROJECT_VERSION}"
        CXX_STANDARD    17
        )

install(TARGETS Borderlands2_Save_LIB
        CONFIGURATIONS Debug RelWithDebInfo
        ARCHIVE
        DESTINATION lib
        COMPONENT Development
        LIBRARY
        DESTINATION lib
        COMPONENT Libraries
        NAMELINK_COMPONENT Development
        PUBLIC_HEADER
        DESTINATION include
        COMPONENT Development
        )
