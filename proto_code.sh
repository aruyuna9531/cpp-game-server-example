rm -rf ./proto_codes
mkdir ./proto_codes
mkdir ./proto_codes/message
protoc --cpp_out=proto_codes ./proto/*.proto