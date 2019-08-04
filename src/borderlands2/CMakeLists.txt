cmake_minimum_required(VERSION 3.14)
cmake_policy(SET CMP0087 NEW)



set(BorderlandsSaveEditor_Borderlands2_LIB_INCLUDE_DIR
        ${BorderlandsSaveEditor_INCLUDE_DIR}/borderlands2
        )

set(BorderlandsSaveEditor_Borderlands2_LIB_PUBLIC_INCLUDE_FILES
        ${BorderlandsSaveEditor_Borderlands2_LIB_INCLUDE_DIR}/borderlands2.hpp
        ${CMAKE_CURRENT_BINARY_DIR}/bl2_save_editor_exports.hpp
        )

set(BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR ${BorderlandsSaveEditor_RESOURCE_DIR}/protobuf)

set(BorderlandsSaveEditor_Borderlands2_LIB_PROTO_FILES
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/BankSlot.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/ChallengeData.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/ChosenVehicleCustomization.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/Color.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/DLCExpansionData.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/GoldenKeys.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/GUID.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/InventorySlotData.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/ItemData.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/ItemMemento.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/LockoutData.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/MissionData.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/MissionPlaythroughData.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/MissionStatus.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/OneOffLevelChallengeData.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/PackedItemData.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/PackedItemDataOptional.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/PackedWeaponData.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/PackedWeaponDataOptional.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/PendingMissionRewards.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/PlayerMark.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/QuickWeaponSlot.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/RegionGameStageData.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/ResourceData.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/SkillData.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/UIPreferencesData.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/WeaponData.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/WeaponMemento.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/WorldDiscoveryData.proto
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_DIR}/WillowTwoPlayerSaveGame.proto
        )

protobuf_generate_cpp(BorderlandsSaveEditor_Borderlands2_LIB_PROTO_SRCS
        BorderlandsSaveEditor_Borderlands2_LIB_PROTO_HDRS
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_FILES}
        )

set(BorderlandsSaveEditor_Borderlands2_LIB_PRIVATE_INCLUDE_FILES
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_HDRS}
        )

set(BorderlandsSaveEditor_Borderlands2_LIB_INCLUDE_FILES
        ${BorderlandsSaveEditor_LIB_PUBLIC_INCLUDE_FILES}
        ${BorderlandsSaveEditor_LIB_PRIVATE_INCLUDE_FILES}
        )

set(BorderlandsSaveEditor_Borderlands2_LIB_SOURCE_FILES
        ${BorderlandsSaveEditor_Borderlands2_LIB_PROTO_SRCS}
        ${CMAKE_CURRENT_SOURCE_DIR}/borderlands2.cpp
        )

set(BorderlandsSaveEditor_Borderlands2_LIB_RESOURCE_FILES)

if (WIN32)
    include(generate_product_version)
    generate_product_version(
            BorderlandsSaveEditor_Borderlands2_VersionFilesOutputVariable
            NAME                "Borderlands2SaveEditor"
            ICON                "${BorderlandsSaveEditor_RESOURCE_DIR}/appicon.ico"
            VERSION_MAJOR       ${CMAKE_PROJECT_VERSION_MAJOR}
            VERSION_MINOR       ${CMAKE_PROJECT_VERSION_MINOR}
            VERSION_PATCH       ${CMAKE_PROJECT_VERSION_PATCH}
            VERSION_REVISION    ${CMAKE_PROJECT_VERSION_TWEAK}
    )
    set(BorderlandsSaveEditor_Borderlands2_LIB_RESOURCE_FILES
            ${BorderlandsSaveEditor_Borderlands2_LIB_RESOURCE_FILES}
            ${BorderlandsSaveEditor_Borderlands2_LIB_VersionFilesOutputVariable}
            )
endif (WIN32)

add_library(BorderlandsSaveEditor_Borderlands2_LIB SHARED
        ${BorderlandsSaveEditor_Borderlands2_LIB_SOURCE_FILES}
        ${BorderlandsSaveEditor_Borderlands2_LIB_INCLUDE_FILES}
        ${BorderlandsSaveEditor_Borderlands2_LIB_RESOURCE_FILES}
        )

generate_export_header(BorderlandsSaveEditor_Borderlands2_LIB
        BASE_NAME                   Borderlands2SaveEditor
        EXPORT_FILE_NAME            bl2_save_editor_exports.hpp
        EXPORT_MACRO_NAME           BORDERLANDS2_SAVE_EDITOR_API
        DEPRECATED_MACRO_NAME       BORDERLANDS2_SAVE_EDITOR_API_DEPRECATED
        NO_EXPORT_MACRO_NAME        BORDERLANDS2_SAVE_EDITOR_API_NO_EXPORT
        NO_DEPRECATED_MACRO_NAME    BORDERLANDS2_SAVE_EDITOR_API_NO_DEPRECATED
        )

target_link_libraries(BorderlandsSaveEditor_Borderlands2_LIB
        PUBLIC
        Borderlands_Common_LIB
        OpenSSL::Crypto
        EXTERNAL_MiniLZO_LIB
        Boost::atomic
        Boost::chrono
        Boost::date_time
        Boost::filesystem
        Boost::log
        Boost::log_setup
        Boost::system
        Boost::thread
        Boost::regex
        ${LIBZIP_LIBRARY}
        ${Protobuf_LIBRARIES}
        )

target_compile_options(BorderlandsSaveEditor_Borderlands2_LIB
        PUBLIC -DBorderlands2SaveEditor_LIB_EXPORTS=1
        )

set_target_properties(BorderlandsSaveEditor_Borderlands2_LIB
        PROPERTIES
        OUTPUT_NAME     "Borderlands2SaveEditor"
        LANGUAGES       CXX
        PUBLIC_HEADERS  "${BorderlandsSaveEditor_Borderlands2_LIB_PUBLIC_INCLUDE_FILES}"
        PRIVATE_HEADERS "${BorderlandsSaveEditor_Borderlands2_LIB_PRIVATE_INCLUDE_FILES}"
        VERSION         "${CMAKE_PROJECT_VERSION}"
        )