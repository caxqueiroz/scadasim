//
// Generated file, do not edit! Created by opp_msgc 4.2 from applications/generic/GenericApplicationMessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "GenericApplicationMessage_m.h"

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




Register_Class(GenericApplicationMessage);

GenericApplicationMessage::GenericApplicationMessage(const char *name, int kind) : cPacket(name,kind)
{
    this->replyLength_var = 0;
    this->replyPerRequest_var = 0;
    this->timeToRespond_var = 0;
    this->last_var = 0;
    this->packetNumber_var = 0;
}

GenericApplicationMessage::GenericApplicationMessage(const GenericApplicationMessage& other) : cPacket()
{
    setName(other.getName());
    operator=(other);
}

GenericApplicationMessage::~GenericApplicationMessage()
{
}

GenericApplicationMessage& GenericApplicationMessage::operator=(const GenericApplicationMessage& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    this->replyLength_var = other.replyLength_var;
    this->replyPerRequest_var = other.replyPerRequest_var;
    this->timeToRespond_var = other.timeToRespond_var;
    this->last_var = other.last_var;
    this->packetNumber_var = other.packetNumber_var;
    return *this;
}

void GenericApplicationMessage::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->replyLength_var);
    doPacking(b,this->replyPerRequest_var);
    doPacking(b,this->timeToRespond_var);
    doPacking(b,this->last_var);
    doPacking(b,this->packetNumber_var);
}

void GenericApplicationMessage::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->replyLength_var);
    doUnpacking(b,this->replyPerRequest_var);
    doUnpacking(b,this->timeToRespond_var);
    doUnpacking(b,this->last_var);
    doUnpacking(b,this->packetNumber_var);
}

int GenericApplicationMessage::getReplyLength() const
{
    return replyLength_var;
}

void GenericApplicationMessage::setReplyLength(int replyLength)
{
    this->replyLength_var = replyLength;
}

int GenericApplicationMessage::getReplyPerRequest() const
{
    return replyPerRequest_var;
}

void GenericApplicationMessage::setReplyPerRequest(int replyPerRequest)
{
    this->replyPerRequest_var = replyPerRequest;
}

double GenericApplicationMessage::getTimeToRespond() const
{
    return timeToRespond_var;
}

void GenericApplicationMessage::setTimeToRespond(double timeToRespond)
{
    this->timeToRespond_var = timeToRespond;
}

bool GenericApplicationMessage::getLast() const
{
    return last_var;
}

void GenericApplicationMessage::setLast(bool last)
{
    this->last_var = last;
}

int GenericApplicationMessage::getPacketNumber() const
{
    return packetNumber_var;
}

void GenericApplicationMessage::setPacketNumber(int packetNumber)
{
    this->packetNumber_var = packetNumber;
}

class GenericApplicationMessageDescriptor : public cClassDescriptor
{
  public:
    GenericApplicationMessageDescriptor();
    virtual ~GenericApplicationMessageDescriptor();

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

Register_ClassDescriptor(GenericApplicationMessageDescriptor);

GenericApplicationMessageDescriptor::GenericApplicationMessageDescriptor() : cClassDescriptor("GenericApplicationMessage", "cPacket")
{
}

GenericApplicationMessageDescriptor::~GenericApplicationMessageDescriptor()
{
}

bool GenericApplicationMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<GenericApplicationMessage *>(obj)!=NULL;
}

const char *GenericApplicationMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int GenericApplicationMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount(object) : 5;
}

unsigned int GenericApplicationMessageDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *GenericApplicationMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "replyLength",
        "replyPerRequest",
        "timeToRespond",
        "last",
        "packetNumber",
    };
    return (field>=0 && field<5) ? fieldNames[field] : NULL;
}

int GenericApplicationMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='r' && strcmp(fieldName, "replyLength")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "replyPerRequest")==0) return base+1;
    if (fieldName[0]=='t' && strcmp(fieldName, "timeToRespond")==0) return base+2;
    if (fieldName[0]=='l' && strcmp(fieldName, "last")==0) return base+3;
    if (fieldName[0]=='p' && strcmp(fieldName, "packetNumber")==0) return base+4;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *GenericApplicationMessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "double",
        "bool",
        "int",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : NULL;
}

const char *GenericApplicationMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int GenericApplicationMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    GenericApplicationMessage *pp = (GenericApplicationMessage *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string GenericApplicationMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    GenericApplicationMessage *pp = (GenericApplicationMessage *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getReplyLength());
        case 1: return long2string(pp->getReplyPerRequest());
        case 2: return double2string(pp->getTimeToRespond());
        case 3: return bool2string(pp->getLast());
        case 4: return long2string(pp->getPacketNumber());
        default: return "";
    }
}

bool GenericApplicationMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    GenericApplicationMessage *pp = (GenericApplicationMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setReplyLength(string2long(value)); return true;
        case 1: pp->setReplyPerRequest(string2long(value)); return true;
        case 2: pp->setTimeToRespond(string2double(value)); return true;
        case 3: pp->setLast(string2bool(value)); return true;
        case 4: pp->setPacketNumber(string2long(value)); return true;
        default: return false;
    }
}

const char *GenericApplicationMessageDescriptor::getFieldStructName(void *object, int field) const
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
        NULL,
        NULL,
    };
    return (field>=0 && field<5) ? fieldStructNames[field] : NULL;
}

void *GenericApplicationMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    GenericApplicationMessage *pp = (GenericApplicationMessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


