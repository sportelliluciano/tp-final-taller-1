#include "modelo/id.h"

namespace modelo {

Id::Id(){
}
Id::~Id(){
}
int Id::nuevo_id(){
    id++;
    return id-1;
}
} // namespace modelo
