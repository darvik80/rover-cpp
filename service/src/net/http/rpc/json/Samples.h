//
// Created by Ivan Kishchenko on 2019-06-09.
//

#pragma once

#include "Helper.h"

BEGIN_DECLARE_DTO(Message)
__DECLARE_DTO_FIELD(int, id)
__DECLARE_DTO_FIELD(std::string, message)

    BEGIN_JSON_UNMARSHAL
            ITEM_JSON_UNMARSHAL(id)
            ITEM_JSON_UNMARSHAL_TAG(message, message)
    END_JSON_UNMARSHAL

    BEGIN_JSON_MARSHAL
            ITEM_JSON_MARSHAL(id)
            ITEM_JSON_MARSHAL_TAG(message, message)
    END_JSON_MARSHAL

END_DECLARE_DTO

BEGIN_DECLARE_DTO(Properties)
__DECLARE_DTO_FIELD(int, id)
__DECLARE_DTO_FIELD(std::string, message)

    BEGIN_JSON_UNMARSHAL
            ITEM_JSON_UNMARSHAL(id)
            ITEM_JSON_UNMARSHAL(message)
    END_JSON_UNMARSHAL

    BEGIN_JSON_MARSHAL
            ITEM_JSON_MARSHAL(id)
            ITEM_JSON_MARSHAL(message)
    END_JSON_MARSHAL

END_DECLARE_DTO

BEGIN_DECLARE_DTO(Controller)
__DECLARE_DTO_FIELD(int, id)
__DECLARE_DTO_FIELD(Properties, config)

    BEGIN_JSON_UNMARSHAL
            ITEM_JSON_UNMARSHAL(id)
            ITEM_JSON_UNMARSHAL(config)
    END_JSON_UNMARSHAL

    BEGIN_JSON_MARSHAL
            ITEM_JSON_MARSHAL(id)
            ITEM_JSON_MARSHAL(config)
    END_JSON_MARSHAL

END_DECLARE_DTO

BEGIN_DECLARE_DTO(TestMessage)
__DECLARE_DTO_FIELD(int, id)
__DECLARE_DTO_FIELD(Message, message)

    BEGIN_JSON_UNMARSHAL
            ITEM_JSON_UNMARSHAL(id)
            ITEM_JSON_UNMARSHAL(message)
    END_JSON_UNMARSHAL

    BEGIN_JSON_MARSHAL
            ITEM_JSON_MARSHAL(id)
            ITEM_JSON_MARSHAL(message)
    END_JSON_MARSHAL
END_DECLARE_DTO
