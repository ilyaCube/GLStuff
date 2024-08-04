#pragma once

#include "Owner.hpp"
#include "Context.hpp"

namespace Library::Threading {

class OwnerAndContext : public Owner, public Context
{
};

}