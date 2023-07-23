#ifndef KAUTIL_LIGHT_ITERATOR_LIGHT_ITERATOR_H
#define KAUTIL_LIGHT_ITERATOR_LIGHT_ITERATOR_H


struct kautil_continuous_itr_iterator; 
kautil_continuous_itr_iterator* kautil_continuous_itr_initialize();
void kautil_continuous_itr_setup(kautil_continuous_itr_iterator* m,void * beg, void * end,int step,int size0,int size1);
void kautil_continuous_itr_finalize(kautil_continuous_itr_iterator* m);
bool kautil_continuous_itr_next(kautil_continuous_itr_iterator* m);
void kautil_continuous_itr_reset(kautil_continuous_itr_iterator* m);
void* kautil_continuous_itr_current(kautil_continuous_itr_iterator* m);

#define kautil_continuous_itr_setup_util(m,type,arg) kautil_continuous_itr_setup(m,(arg).begin().base(),(arg).end().base(),sizeof(type::value_type),sizeof(type),sizeof( decltype(arg) ))
#define kautil_continuous_itr_setup_util_p(m,type,arg) kautil_continuous_itr_setup(m,(arg)->begin().base(),(arg)->end().base(),sizeof(type::value_type),sizeof(type),sizeof( decltype(*(arg)) ))


struct kautil_non_continuous_iterator; 
kautil_non_continuous_iterator* kautil_non_continuous_itr_initialize();
void kautil_non_continuous_itr_finalize(kautil_non_continuous_iterator* m);
void kautil_non_continuous_itr_setup(kautil_non_continuous_iterator* m,void * obj,void * (*new_begin)(void * obj),void * (*new_end)(void * obj),void (*del)(void * ),void (*step)(void * element_as_voidptr),bool (*is_end)(void * cur,void * end),int size0,int size1);
bool kautil_non_continuous_itr_next(kautil_non_continuous_iterator* m);
void kautil_non_continuous_itr_reset(kautil_non_continuous_iterator* m);
void* kautil_non_continuous_itr_current(kautil_non_continuous_iterator* m);
#define kautil_non_continuous_itr_setup_util_p(itr,type,ptr) kautil_non_continuous_itr_setup(\
    itr\
    ,ptr\
    ,[](void * o)->void*{ return new type::iterator(reinterpret_cast<type*>(o)->begin()); }\
    ,[](void * o)->void*{ return new type::iterator(reinterpret_cast<type*>(o)->end()); }\
    ,[](void * itr){  delete reinterpret_cast<type::iterator*>(itr); }\
    ,[](void* v){ ++(*reinterpret_cast<type::iterator*>(v)); }\
    ,[](void* cur,void* end){ return *reinterpret_cast<type::iterator*>(cur) != *reinterpret_cast<type::iterator*>(end); },sizeof(type),sizeof(decltype(*(ptr))));

#define kautil_non_continuous_itr_setup_util_declared_p(itr,prefix,ptr) kautil_non_continuous_itr_setup(itr, (ptr) ,prefix##_begin ,prefix##_end ,prefix##_delete ,prefix##_next ,prefix##_is_end,prefix##_object_size(),sizeof(decltype( *(ptr) )))
#define kautil_non_continuous_itr_setup_util(itr,type,obj) kautil_non_continuous_itr_setup_util_p(itr,type,&obj)
#define kautil_non_continuous_itr_setup_util_declared(itr,type,obj) kautil_non_continuous_itr_setup_util_declared_p(itr,type,&obj)

namespace kautil{
namespace light_iterator{


struct continuous_internal;
struct Continuous{
    friend Continuous* Continuous_internal_factory();
    ~Continuous();
    static Continuous factory(void * beg, void * end,int step,int size0,int size1);
    void set(void * beg, void * end,int step,int size0,int size1) const;
    bool next() const;
    void reset()const; 
    void * current() const;
private:
    Continuous();
    continuous_internal * m=0;
};


#define continuous_iterator(type,arg) kautil::light_iterator::Continuous::factory((arg).begin().base(),(arg).end().base(), sizeof(type::value_type),sizeof(type),sizeof( decltype(arg) ))
#define continuous_iterator_p(type,arg) kautil::light_iterator::Continuous::factory((arg)->begin().base(),(arg)->end().base(), sizeof(type::value_type),sizeof(type),sizeof( decltype(*(arg)) ))



struct non_continuous_internal;
struct NonContinuous{
    friend NonContinuous* NonContinuous_internal_factory();
    ~NonContinuous();
    NonContinuous(NonContinuous && r);
    bool next() const; 
    static NonContinuous factory(void * obj,void * (*new_begin)(void * obj),void * (*new_end)(void * obj),void (*del)(void * ),void (*step)(void * element_as_voidptr),bool (*is_end)(void * cur,void * end),int size0,int size1);
    void set(void * obj,void * (*new_begin)(void * obj),void * (*new_end)(void * obj),void (*del)(void * ),void (*step)(void * element_as_voidptr),bool (*is_end)(void * cur,void * end),int size0,int size1); 
    void reset() const ;
    void* current() const ;
    
private:
    NonContinuous(NonContinuous const& )=delete;
    NonContinuous& operator=(NonContinuous const&)=delete;
    NonContinuous() ;
    non_continuous_internal *m=0;
};



#define noncontinuous_iterator_p(type,ptr) kautil::light_iterator::NonContinuous::factory(\
    ptr\
    ,[](void * o)->void*{ return new type::iterator(reinterpret_cast<type*>(o)->begin()); }\
    ,[](void * o)->void*{ return new type::iterator(reinterpret_cast<type*>(o)->end()); }\
    ,[](void * itr){  delete reinterpret_cast<type::iterator*>(itr); }\
    ,[](void* v){ ++(*reinterpret_cast<type::iterator*>(v)); }\
    ,[](void* cur,void* end){ return *reinterpret_cast<type::iterator*>(cur) != *reinterpret_cast<type::iterator*>(end); },sizeof(type),sizeof(decltype(*(ptr))));

#define noncontinuous_iterator(type,obj) noncontinuous_iterator_p(type,&obj)


#define noncontinuous_generate_callback_functions(prefix,type)\
void * prefix##_begin(void * o){ return new type::iterator(reinterpret_cast<type*>(o)->begin()); }\
void * prefix##_end(void * o){ return new type::iterator(reinterpret_cast<type*>(o)->end()); }\
void prefix##_delete(void * itr){  delete reinterpret_cast<type::iterator*>(itr); }\
void prefix##_next(void* v){ ++(*reinterpret_cast<type::iterator*>(v)); }\
bool prefix##_is_end(void* cur,void* end){ return *reinterpret_cast<type::iterator*>(cur) != *reinterpret_cast<type::iterator*>(end); }\
int32_t prefix##_object_size(){ return sizeof(type); }

#define noncontinuous_iterator_declared_p(prefix,ptr) kautil::light_iterator::NonContinuous::factory( ptr ,prefix##_begin ,prefix##_end ,prefix##_delete ,prefix##_next ,prefix##_is_end,prefix##_object_size(),sizeof(decltype(*(ptr))))
#define noncontinuous_iterator_declared(prefix,obj) kautil::light_iterator::NonContinuous::factory( (&obj) ,prefix##_begin ,prefix##_end ,prefix##_delete ,prefix##_next ,prefix##_is_end,prefix##_object_size(),sizeof(decltype(obj)))


} //namespace light_iterator{
} //namespace kautil{



#endif