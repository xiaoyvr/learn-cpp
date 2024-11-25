#pragma once
#include <regex>

#include "testinghelper.h"
#include "EntityRelationshipType.h"

namespace testing::naming {
    
    struct OperationTypeN : named<magl::entity::OperationType> {
        OperationTypeN(magl::entity::OperationType op) : named(op) {}

        operator std::string() const override {
            auto str = magl::entity::OperationTypeToString(e);
            str.erase(std::remove_if(str.begin(), str.end(), [](unsigned char ch) { return !(std::isalnum(ch) || ch == '_'); }), str.end());
            return str;
        }

        operator magl::entity::OperationType () const override {
            return e;
        }
    };

    struct RelationshipTypeN : named<magl::entity::RelationshipType> {
        RelationshipTypeN(magl::entity::RelationshipType op) : named(op) {}

        operator std::string() const override {
            return magl::entity::RelationshipTypeToString(e);
        }
        operator magl::entity::RelationshipType() const override {
            return e;
        }
    };

    OperationTypeN _n(magl::entity::OperationType op) { return OperationTypeN(op); }
    RelationshipTypeN _n(magl::entity::RelationshipType op) {return RelationshipTypeN(op);}
}
