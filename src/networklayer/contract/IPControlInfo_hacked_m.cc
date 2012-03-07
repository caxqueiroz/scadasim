//
// Generated file, do not edit! Created by opp_msgc 4.2 from networklayer/contract/IPControlInfo_hacked.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "IPControlInfo_hacked_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




IPControlInfo_hacked::IPControlInfo_hacked() : IPv4ControlInfo()
{
    this->attackTag_var = 0;
}

IPControlInfo_hacked::IPControlInfo_hacked(const IPControlInfo_hacked& other) : IPv4ControlInfo(other)
{
    copy(other);
}

IPControlInfo_hacked::~IPControlInfo_hacked()
{
}

IPControlInfo_hacked& IPControlInfo_hacked::operator=(const IPControlInfo_hacked& other)
{
    if (this==&other) return *this;
    IPv4ControlInfo::operator=(other);
    copy(other);
    return *this;
}

void IPControlInfo_hacked::copy(const IPControlInfo_hacked& other)
{
    this->attackTag_var = other.attackTag_var;
}

void IPControlInfo_hacked::parsimPack(cCommBuffer *b)
{
    doPacking(b,(IPv4ControlInfo&)*this);
    doPacking(b,this->attackTag_var);
}

void IPControlInfo_hacked::parsimUnpack(cCommBuffer *b)
{
    doUnpacking(b,(IPv4ControlInfo&)*this);
    doUnpacking(b,this->attackTag_var);
}

short IPControlInfo_hacked::getAttackTag() const
{
    return attackTag_var;
}

void IPControlInfo_hacked::setAttackTag(short attackTag)
{
    this->attackTag_var = attackTag;
}

class IPControlInfo_hackedDescriptor : public cClassDescriptor
{
  public:
    IPControlInfo_hackedDescriptor();
    virtual ~IPControlInfo_hackedDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(IPControlInfo_hackedDescriptor);

IPControlInfo_hackedDescriptor::IPControlInfo_hackedDescriptor() : cClassDescriptor("IPControlInfo_hacked", "IPv4ControlInfo")
{
}

IPControlInfo_hackedDescriptor::~IPControlInfo_hackedDescriptor()
{
}

bool IPControlInfo_hackedDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<IPControlInfo_hacked *>(obj)!=NULL;
}

const char *IPControlInfo_hackedDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int IPControlInfo_hackedDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount(object) : 1;
}

unsigned int IPControlInfo_hackedDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *IPControlInfo_hackedDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "attackTag",
    };
    return (field>=0 && field<1) ? fieldNames[field] : NULL;
}

int IPControlInfo_hackedDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='a' && strcmp(fieldName, "attackTag")==0) return base+0;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *IPControlInfo_hackedDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "short",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : NULL;
}

const char *IPControlInfo_hackedDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int IPControlInfo_hackedDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    IPControlInfo_hacked *pp = (IPControlInfo_hacked *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string IPControlInfo_hackedDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    IPControlInfo_hacked *pp = (IPControlInfo_hacked *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getAttackTag());
        default: return "";
    }
}

bool IPControlInfo_hackedDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    IPControlInfo_hacked *pp = (IPControlInfo_hacked *)object; (void)pp;
    switch (field) {
        case 0: pp->setAttackTag(string2long(value)); return true;
        default: return false;
    }
}

const char *IPControlInfo_hackedDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
    };
    return (field>=0 && field<1) ? fieldStructNames[field] : NULL;
}

void *IPControlInfo_hackedDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    IPControlInfo_hacked *pp = (IPControlInfo_hacked *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


