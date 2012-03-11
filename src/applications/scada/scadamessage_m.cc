//
// Generated file, do not edit! Created by opp_msgc 4.2 from applications/scada/scadamessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "scadamessage_m.h"

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




Register_Class(scadamessage);

scadamessage::scadamessage(const char *name, int kind) : cPacket(name,kind)
{
    this->timeToRespond_var = 0;
    this->last_var = 0;
    this->packetNumber_var = 0;
}

scadamessage::scadamessage(const scadamessage& other) : cPacket(other)
{
    copy(other);
}

scadamessage::~scadamessage()
{
}

scadamessage& scadamessage::operator=(const scadamessage& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    copy(other);
    return *this;
}

void scadamessage::copy(const scadamessage& other)
{
    this->timeToRespond_var = other.timeToRespond_var;
    this->last_var = other.last_var;
    this->packetNumber_var = other.packetNumber_var;
}

void scadamessage::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->timeToRespond_var);
    doPacking(b,this->last_var);
    doPacking(b,this->packetNumber_var);
}

void scadamessage::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->timeToRespond_var);
    doUnpacking(b,this->last_var);
    doUnpacking(b,this->packetNumber_var);
}

double scadamessage::getTimeToRespond() const
{
    return timeToRespond_var;
}

void scadamessage::setTimeToRespond(double timeToRespond)
{
    this->timeToRespond_var = timeToRespond;
}

bool scadamessage::getLast() const
{
    return last_var;
}

void scadamessage::setLast(bool last)
{
    this->last_var = last;
}

int scadamessage::getPacketNumber() const
{
    return packetNumber_var;
}

void scadamessage::setPacketNumber(int packetNumber)
{
    this->packetNumber_var = packetNumber;
}

class scadamessageDescriptor : public cClassDescriptor
{
  public:
    scadamessageDescriptor();
    virtual ~scadamessageDescriptor();

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

Register_ClassDescriptor(scadamessageDescriptor);

scadamessageDescriptor::scadamessageDescriptor() : cClassDescriptor("scadamessage", "cPacket")
{
}

scadamessageDescriptor::~scadamessageDescriptor()
{
}

bool scadamessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<scadamessage *>(obj)!=NULL;
}

const char *scadamessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int scadamessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int scadamessageDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *scadamessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "timeToRespond",
        "last",
        "packetNumber",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int scadamessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='t' && strcmp(fieldName, "timeToRespond")==0) return base+0;
    if (fieldName[0]=='l' && strcmp(fieldName, "last")==0) return base+1;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetNumber")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *scadamessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "double",
        "bool",
        "int",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *scadamessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int scadamessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    scadamessage *pp = (scadamessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string scadamessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    scadamessage *pp = (scadamessage *)object; (void)pp;
    switch (field) {
        case 0: return double2string(pp->getTimeToRespond());
        case 1: return bool2string(pp->getLast());
        case 2: return long2string(pp->getPacketNumber());
        default: return "";
    }
}

bool scadamessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    scadamessage *pp = (scadamessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setTimeToRespond(string2double(value)); return true;
        case 1: pp->setLast(string2bool(value)); return true;
        case 2: pp->setPacketNumber(string2long(value)); return true;
        default: return false;
    }
}

const char *scadamessageDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<3) ? fieldStructNames[field] : NULL;
}

void *scadamessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    scadamessage *pp = (scadamessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


