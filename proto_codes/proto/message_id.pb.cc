// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: proto/message_id.proto

#include "proto/message_id.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
namespace proto {
}  // namespace proto
static constexpr ::PROTOBUF_NAMESPACE_ID::Metadata* file_level_metadata_proto_2fmessage_5fid_2eproto = nullptr;
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_proto_2fmessage_5fid_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_proto_2fmessage_5fid_2eproto = nullptr;
const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_proto_2fmessage_5fid_2eproto::offsets[1] = {};
static constexpr ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema* schemas = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::Message* const* file_default_instances = nullptr;

const char descriptor_table_protodef_proto_2fmessage_5fid_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\026proto/message_id.proto\022\005proto*\376\002\n\tMess"
  "ageId\022\014\n\010Msg_None\020\000\022\020\n\014Msg_CS_Login\020\001\022\020\n"
  "\014Msg_SC_Login\020\002\022\022\n\016Msg_CS_Message\020\003\022\022\n\016M"
  "sg_SC_Message\020\004\022\033\n\027Msg_SC_DisconnectNoti"
  "fy\020\005\022\017\n\013Msg_CS_Chat\020\006\022\017\n\013Msg_SC_Chat\020\007\022\030"
  "\n\024Msg_SC_NewChatNotify\020\010\022\r\n\tMsg_BEGIN\020d\022"
  "\017\n\013Msg_Connect\020e\022\021\n\rMsg_Heartbeat\020f\022\020\n\014M"
  "sg_JoinRoom\020g\022\020\n\014Msg_Progress\020h\022\r\n\tMsg_R"
  "eady\020i\022\r\n\tMsg_Start\020j\022\r\n\tMsg_Frame\020k\022\r\n\t"
  "Msg_Input\020l\022\016\n\nMsg_Result\020m\022\r\n\tMsg_Close"
  "\020n\022\014\n\007Msg_END\020\377\001B\025Z\023proto_codes/messageb"
  "\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_proto_2fmessage_5fid_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_proto_2fmessage_5fid_2eproto_sccs[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_proto_2fmessage_5fid_2eproto_once;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_proto_2fmessage_5fid_2eproto = {
  false, false, descriptor_table_protodef_proto_2fmessage_5fid_2eproto, "proto/message_id.proto", 447,
  &descriptor_table_proto_2fmessage_5fid_2eproto_once, descriptor_table_proto_2fmessage_5fid_2eproto_sccs, descriptor_table_proto_2fmessage_5fid_2eproto_deps, 0, 0,
  schemas, file_default_instances, TableStruct_proto_2fmessage_5fid_2eproto::offsets,
  file_level_metadata_proto_2fmessage_5fid_2eproto, 0, file_level_enum_descriptors_proto_2fmessage_5fid_2eproto, file_level_service_descriptors_proto_2fmessage_5fid_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_proto_2fmessage_5fid_2eproto = (static_cast<void>(::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_proto_2fmessage_5fid_2eproto)), true);
namespace proto {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* MessageId_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_proto_2fmessage_5fid_2eproto);
  return file_level_enum_descriptors_proto_2fmessage_5fid_2eproto[0];
}
bool MessageId_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 100:
    case 101:
    case 102:
    case 103:
    case 104:
    case 105:
    case 106:
    case 107:
    case 108:
    case 109:
    case 110:
    case 255:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace proto
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>