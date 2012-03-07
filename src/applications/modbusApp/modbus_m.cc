//
// Generated file, do not edit! Created by opp_msgc 4.2 from applications/modbusApp/modbus.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "modbus_m.h"

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




Register_Class(Modbus);

Modbus::Modbus(const char *name, int kind) : cPacket(name,kind)
{
    this->length_var = 0;
    pdu_arraysize = 0;
    this->pdu_var = 0;
    this->reply_delay_var = 0;
    this->close_conn_var = 0;
}

Modbus::Modbus(const Modbus& other) : cPacket(other)
{
    pdu_arraysize = 0;
    this->pdu_var = 0;
    copy(other);
}

Modbus::~Modbus()
{
    delete [] pdu_var;
}

Modbus& Modbus::operator=(const Modbus& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    copy(other);
    return *this;
}

void Modbus::copy(const Modbus& other)
{
    this->length_var = other.length_var;
    delete [] this->pdu_var;
    this->pdu_var = (other.pdu_arraysize==0) ? NULL : new uint8_t[other.pdu_arraysize];
    pdu_arraysize = other.pdu_arraysize;
    for (unsigned int i=0; i<pdu_arraysize; i++)
        this->pdu_var[i] = other.pdu_var[i];
    this->reply_delay_var = other.reply_delay_var;
    this->close_conn_var = other.close_conn_var;
}

void Modbus::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->length_var);
    b->pack(pdu_arraysize);
    doPacking(b,this->pdu_var,pdu_arraysize);
    doPacking(b,this->reply_delay_var);
    doPacking(b,this->close_conn_var);
}

void Modbus::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->length_var);
    delete [] this->pdu_var;
    b->unpack(pdu_arraysize);
    if (pdu_arraysize==0) {
        this->pdu_var = 0;
    } else {
        this->pdu_var = new uint8_t[pdu_arraysize];
        doUnpacking(b,this->pdu_var,pdu_arraysize);
    }
    doUnpacking(b,this->reply_delay_var);
    doUnpacking(b,this->close_conn_var);
}

int Modbus::getLength() const
{
    return length_var;
}

void Modbus::setLength(int length)
{
    this->length_var = length;
}

void Modbus::setPduArraySize(unsigned int size)
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

unsigned int Modbus::getPduArraySize() const
{
    return pdu_arraysize;
}

uint8_t Modbus::getPdu(unsigned int k) const
{
    if (k>=pdu_arraysize) throw cRuntimeError("Array of size %d indexed by %d", pdu_arraysize, k);
    return pdu_var[k];
}

void Modbus::setPdu(unsigned int k, uint8_t pdu)
{
    if (k>=pdu_arraysize) throw cRuntimeError("Array of size %d indexed by %d", pdu_arraysize, k);
    this->pdu_var[k] = pdu;
}

double Modbus::getReply_delay() const
{
    return reply_delay_var;
}

void Modbus::setReply_delay(double reply_delay)
{
    this->reply_delay_var = reply_delay;
}

bool Modbus::getClose_conn() const
{
    return close_conn_var;
}

void Modbus::setClose_conn(bool close_conn)
{
    this->close_conn_var = close_conn;
}

class ModbusDescriptor : public cClassDescriptor
{
  public:
    ModbusDescriptor();
    virtual ~ModbusDescriptor();

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

Register_ClassDescriptor(ModbusDescriptor);

ModbusDescriptor::ModbusDescriptor() : cClassDescriptor("Modbus", "cPacket")
{
}

ModbusDescriptor::~ModbusDescriptor()
{
}

bool ModbusDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<Modbus *>(obj)!=NULL;
}

const char *ModbusDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int ModbusDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 4+basedesc->getFieldCount(object) : 4;
}

unsigned int ModbusDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<4) ? fieldTypeFlags[field] : 0;
}

const char *ModbusDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "length",
        "pdu",
        "reply_delay",
        "close_conn",
    };
    return (field>=0 && field<4) ? fieldNames[field] : NULL;
}

int ModbusDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='l' && strcmp(fieldName, "length")==0) return base+0;
    if (fieldName[0]=='p' && strcmp(fieldName, "pdu")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "reply_delay")==0) return base+2;
    if (fieldName[0]=='c' && strcmp(fieldName, "close_conn")==0) return base+3;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *ModbusDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "uint8_t",
        "double",
        "bool",
    };
    return (field>=0 && field<4) ? fieldTypeStrings[field] : NULL;
}

const char *ModbusDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int ModbusDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    Modbus *pp = (Modbus *)object; (void)pp;
    switch (field) {
        case 1: return pp->getPduArraySize();
        default: return 0;
    }
}

std::string ModbusDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    Modbus *pp = (Modbus *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getLength());
        case 1: return ulong2string(pp->getPdu(i));
        case 2: return double2string(pp->getReply_delay());
        case 3: return bool2string(pp->getClose_conn());
        default: return "";
    }
}

bool ModbusDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    Modbus *pp = (Modbus *)object; (void)pp;
    switch (field) {
        case 0: pp->setLength(string2long(value)); return true;
        case 1: pp->setPdu(i,string2ulong(value)); return true;
        case 2: pp->setReply_delay(string2double(value)); return true;
        case 3: pp->setClose_conn(string2bool(value)); return true;
        default: return false;
    }
}

const char *ModbusDescriptor::getFieldStructName(void *object, int field) const
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
    };
    return (field>=0 && field<4) ? fieldStructNames[field] : NULL;
}

void *ModbusDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    Modbus *pp = (Modbus *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


