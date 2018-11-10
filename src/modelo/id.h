#ifndef _ID_H_
#define _ID_H_

namespace modelo {

class Id{
    private:
    int id = 0;

    public:
    Id();
    ~Id();
    int nuevo_id();
};
}
#endif