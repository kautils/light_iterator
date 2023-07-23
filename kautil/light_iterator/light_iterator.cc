
#include "light_iterator.h"
#include <stdint.h>
#include <bits\move.h>
#include <exception>

struct non_continuous_typecheck_exception : std::exception{
    const char*what() const noexcept{
        static const char * kMsg = "non_continuous_iterator : different type is detected. container_type::iterator and decltype(VALUE_YOU_SPECIFIED) must be the same.";
        return kMsg;
    }
};
struct continuous_typecheck_exception : std::exception{
    const char*what() const noexcept{
        static const char * kMsg = "continuous_iterator : different type is detected. container_type::iterator and decltype(VALUE_YOU_SPECIFIED) must be the same.";
        return kMsg;
    }
};


namespace kautil{
namespace light_iterator{


#define __i(v) reinterpret_cast<uintptr_t>(v)
#define __v(v) reinterpret_cast<void*>(v)

struct continuous_internal{
    void * end = 0;
    void * beg = 0;
    void * cur=0;
    int step=1;
    bool fst = true;
};

Continuous::Continuous() : m(new continuous_internal){}
Continuous::~Continuous(){ delete m; }
Continuous Continuous::factory(void * beg, void * end,int step,int size0,int size1){
    auto res= Continuous();
    res.set(beg,end,step,size0,size1);
    return res;
}

void Continuous::set(void * beg, void * end,int step,int size0,int size1) const{
    if(size0!=size1) throw continuous_typecheck_exception{};
    m->beg = beg;
    m->end = end;
    m->cur = beg;
    m->step=step;
}
bool Continuous::next() const{
    if(m->fst)m->fst=false;
    else m->cur= __v(__i(m->cur) + m->step);
    return m->cur < m->end;
}
void Continuous::reset()const { m->fst = true;m->cur = m->beg; }
void * Continuous::current() const{ return m->cur; }
Continuous* Continuous_internal_factory(){ return new Continuous{}; }
    

struct non_continuous_internal{
    void * obj = 0;
    void * beg = 0;
    void * end = 0;
    void * cur=0;
    void * (*new_begin)();
    void * (*new_end)();
    void  (*del)(void *);
    void (*step)(void * element_as_voidptr);
    bool (*is_end)(void * cur,void * end);
    bool fst = true;
};

NonContinuous::~NonContinuous(){ 
    if(m){ m->del(m->beg);m->del(m->end);delete m; }
}


bool NonContinuous::next() const {
    if(m->fst) m->fst=false;
    else m->step(m->cur);
    return m->is_end(m->cur,m->end);
}



NonContinuous NonContinuous::factory(void * obj,void * (*new_begin)(void * obj),void * (*new_end)(void * obj),void (*del)(void * )
         ,void (*step)(void * element_as_voidptr)
         ,bool (*is_end)(void * cur,void * end)
         ,int size0,int size1
        ){
    auto res=NonContinuous{};
    res.set(obj,new_begin,new_end,del,step,is_end,size0,size1);
    return std::move(res);
}

void NonContinuous::set(void * obj
         ,void * (*new_begin)(void * obj)
         ,void * (*new_end)(void * obj)
         ,void (*del)(void * )
         ,void (*step)(void * element_as_voidptr)
         ,bool (*is_end)(void * cur,void * end)
        ,int size0,int size1
         ) {
    if(size0!=size1) throw non_continuous_typecheck_exception{};
    
    m->del= del;
    m->del(m->beg);
    m->del(m->end);
    m->beg= new_begin(obj);
    m->end= new_end(obj);
    m->step= step;
    m->is_end= is_end;
    m->cur= m->beg;
}

void NonContinuous::reset() const { m->fst=true;m->cur=m->beg; }
void* NonContinuous::current() const { return m->cur; }
NonContinuous::NonContinuous() : m(new non_continuous_internal){}
NonContinuous::NonContinuous(NonContinuous && r) : m(r.m){ r.m=nullptr; }

NonContinuous *NonContinuous_internal_factory() { return new NonContinuous; }


} //namespace light_iterator{
} //namespace kautil{


#define to_c_type(v)  reinterpret_cast<kautil_continuous_itr_iterator*>(v)
#define _c(v)  reinterpret_cast<kautil::light_iterator::Continuous*>(v)


struct kautil_continuous_itr_iterator{}; 

//struct kautil_continuous_itr_iterator{};
kautil_continuous_itr_iterator* kautil_continuous_itr_initialize(){ return to_c_type(kautil::light_iterator::Continuous_internal_factory()); }
void kautil_continuous_itr_setup(kautil_continuous_itr_iterator* m,void * beg, void * end,int step,int size0,int size1){ _c(m)->set(beg,end,step,size0,size1); }
void kautil_continuous_itr_finalize(kautil_continuous_itr_iterator* m){ delete _c(m); }
bool kautil_continuous_itr_next(kautil_continuous_itr_iterator* m){ return _c(m)->next(); }
void kautil_continuous_itr_reset(kautil_continuous_itr_iterator* m){ _c(m)->reset(); }
void* kautil_continuous_itr_current(kautil_continuous_itr_iterator* m){ return _c(m)->current(); }




#define to_nc_type(v)  reinterpret_cast<kautil_non_continuous_iterator*>(v)
#define _nc(v)  reinterpret_cast<kautil::light_iterator::NonContinuous*>(v)

struct kautil_non_continuous_iterator{}; 
kautil_non_continuous_iterator* kautil_non_continuous_itr_initialize(){ return to_nc_type(kautil::light_iterator::NonContinuous_internal_factory()); }
void kautil_non_continuous_itr_finalize(kautil_non_continuous_iterator* m){ delete _nc(m); }
void kautil_non_continuous_itr_setup(kautil_non_continuous_iterator* m,void * obj,void * (*new_begin)(void * obj),void * (*new_end)(void * obj),void (*del)(void * ),void (*step)(void * element_as_voidptr),bool (*is_end)(void * cur,void * end),int size0,int size1){ _nc(m)->set(obj,new_begin,new_end,del,step,is_end,size0,size1); }
bool kautil_non_continuous_itr_next(kautil_non_continuous_iterator* m){ return _nc(m)->next(); }
void kautil_non_continuous_itr_reset(kautil_non_continuous_iterator* m){ _nc(m)->reset(); }
void* kautil_non_continuous_itr_current(kautil_non_continuous_iterator* m){ return _nc(m)->current(); }
