//
// Generated file, do not edit! Created by opp_msgc 4.2 from applications/modbusApp/ModbusMessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "ModbusMessage_m.h"

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




Register_Class(ModbusMessage);

ModbusMessage::ModbusMessage(const char *name, int kind) : cPacket(name,kind)
{
    pdu_arraysize = 0;
    this->pdu_var = 0;
    this->replyDelay_var = 0;
    this->closeConn_var = 0;
}

ModbusMessage::ModbusMessage(const ModbusMessage& other) : cPacket(other)
{
    pdu_arraysize = 0;
    this->pdu_var = 0;
    copy(other);
}

ModbusMessage::~ModbusMessage()
{
    delete [] pdu_var;
}

ModbusMessage& ModbusMessage::operator=(const ModbusMessage& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    copy(other);
    return *this;
}

void ModbusMessage::copy(const ModbusMessage& other)
{
    delete [] this->pdu_var;
    this->pdu_var = (other.pdu_arraysize==0) ? NULL : new uint8_t[other.pdu_arraysize];
    pdu_arraysize = other.pdu_arraysize;
    for (unsigned int i=0; i<pdu_arraysize; i++)
        this->pdu_var[i] = other.pdu_var[i];
    this->replyDelay_var = other.replyDelay_var;
    this->closeConn_var = other.closeConn_var;
}

void ModbusMessage::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    b->pack(pdu_arraysize);
    doPacking(b,this->pdu_var,pdu_arraysize);
    doPacking(b,this->replyDelay_var);
    doPacking(b,this->closeConn_var);
}

void ModbusMessage::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    delete [] this->pdu_var;
    b->unpack(pdu_arraysize);
    if (pdu_arraysize==0) {
        this->pdu_var = 0;
    } else {
        this->pdu_var = new uint8_t[pdu_arraysize];
        doUnpacking(b,this->pdu_var,pdu_arraysize);
    }
    doUnpacking(b,this->replyDelay_var);
    doUnpacking(b,this->closeConn_var);
}

void ModbusMessage::setPduArraySize(unsigned int size)
{
    uint8_t *pdu_var2 = (size==0) ? NULL : new uint8_t[size];
    unsigned int sz = pdu_arraysize < size ? pdu_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        pdu_var2[i] = this->pdu_var[i];
    for (unsigned int i=sz; i<size; i++)
        pdu_var2[i] = 0;
    pdu_arraysize = size;
    delete [] this->pdu_var;
    this->pdu_var = pdu_var2;
}

unsigned int ModbusMessage::getPduArraySize() const
{
    return pdu_arraysize;
}

uint8_t ModbusMessage::getPdu(unsigned int k) const
{
    if (k>=pdu_arraysize) throw cRuntimeError("Array of size %d indexed by %d", pdu_arraysize, k);
    return pdu_var[k];
}

void ModbusMessage::setPdu(unsigned int k, uint8_t pdu)
{
    if (k>=pdu_arraysize) throw cRuntimeError("Array of size %d indexed by %d", pdu_arraysize, k);
    this->pdu_var[k] = pdu;
}

double ModbusMessage::getReplyDelay() const
{
    return replyDelay_var;
}

void ModbusMessage::setReplyDelay(double replyDelay)
{
    this->replyDelay_var = replyDelay;
}

bool ModbusMessage::getCloseConn() const
{
    return closeConn_var;
}

void ModbusMessage::setCloseConn(bool closeConn)
{
    this->closeConn_var = closeConn;
}

class ModbusMessageDescriptor : public cClassDescriptor
{
  public:
    ModbusMessageDescriptor();
    virtual ~ModbusMessageDescriptor();

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

Register_ClassDescriptor(ModbusMessageDescriptor);

ModbusMessageDescriptor::ModbusMessageDescriptor() : cClassDescriptor("ModbusMessage", "cPacket")
{
}

ModbusMessageDescriptor::~ModbusMessageDescriptor()
{
}

bool ModbusMessageDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<ModbusMessage *>(obj)!=NULL;
}

const char *ModbusMessageDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ModbusMessageDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 3+basedesc->getFieldCount(object) : 3;
}

unsigned int ModbusMessageDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<3) ? fieldTypeFlags[field] : 0;
}

const char *ModbusMessageDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "pdu",
        "replyDelay",
        "closeConn",
    };
    return (field>=0 && field<3) ? fieldNames[field] : NULL;
}

int ModbusMessageDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "pdu")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "replyDelay")==0) return base+1;
    if (fieldName[0]=='c' && strcmp(fieldName, "closeConn")==0) return base+2;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ModbusMessageDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "uint8_t",
        "double",
        "bool",
    };
    return (field>=0 && field<3) ? fieldTypeStrings[field] : NULL;
}

const char *ModbusMessageDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int ModbusMessageDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    ModbusMessage *pp = (ModbusMessage *)object; (void)pp;
    switch (field) {
        case 0: return pp->getPduArraySize();
        default: return 0;
    }
}

std::string ModbusMessageDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    ModbusMessage *pp = (ModbusMessage *)object; (void)pp;
    switch (field) {
        case 0: return ulong2string(pp->getPdu(i));
        case 1: return double2string(pp->getReplyDelay());
        case 2: return bool2string(pp->getCloseConn());
        default: return "";
    }
}

bool ModbusMessageDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    ModbusMessage *pp = (ModbusMessage *)object; (void)pp;
    switch (field) {
        case 0: pp->setPdu(i,string2ulong(value)); return true;
        case 1: pp->setReplyDelay(string2double(value)); return true;
        case 2: pp->setCloseConn(string2bool(value)); return true;
        default: return false;
    }
}

const char *ModbusMessageDescriptor::getFieldStructName(void *object, int field) const
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

void *ModbusMessageDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    ModbusMessage *pp = (ModbusMessage *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


