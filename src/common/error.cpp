#include "error.h"

namespace jimdb
{
    namespace error
    {

        const char* ErrorCode::nameOf[] =
        {
            "parse_error_handshake",
            "invalid_json_request",
            "missing_type_or_data_request",
            "type_or_data_wrong_type_request",

            "oid_not_found_find",
            "invalid_oid_find",
            "missing_oid_find",

            "invalid_oid_delete",
            "oid_not_found_delete",
            "missing_oid_delete"
        };

        static_assert(sizeof(ErrorCode::nameOf) / sizeof(char*) == ErrorCode::ERROR_CODES_SIZE, "sizes do not match");
    }
}