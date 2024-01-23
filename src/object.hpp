/**
 * Contains the declarations of OD objects and entries.
 */
#pragma once
#include <cstdint>
#include <functional>

#include "enums.hpp"
#include "unions.hpp"
#define OBJECT_INDEX_COUNT 0

namespace CANopen {

class IObject;

class ISubObject {
   public:
    virtual MetaBitfield getMetadata() = 0;
    virtual uint16_t getSize() = 0;
    virtual uint8_t* getDefaultValue() = 0;
    virtual void getData(uint8_t* buffer, uint16_t size) = 0;
    virtual void setData(uint8_t* buffer, uint16_t size) = 0;
};

template <typename T>
class LocalSubObject : public ISubObject {
   private:
    uint8_t subindex;
    MetaBitfield metaData;
    T defaultValue;
    T value;
    IObject& object;

   public:
    LocalSubObject(uint16_t index, uint8_t subindex, MetaBitfield metaData,
                   uint8_t* defaultValue)
        : index(index),
          subindex(subindex),
          metaData(metaData),
          defaultValue(defaultValue),
          object(object) {}
    MetaBitfield getMetadata() { return metaData; }
    uint16_t getSize() { return sizeof(T); }
    uint8_t* getDefaultValue();
    void getData(uint8_t* buffer, uint16_t size) {
        memcpy(buffer, &value, size);
    }
    void setData(uint8_t* buffer, uint16_t size) {
        memcpy(&value, buffer, size);
    }
};

template <typename T>
class RemoteSubObject : public ISubObject {
   private:
    uint8_t subindex;
    std::function<T(uint16_t, uint8_t)> getRemoteData;
    std::function<void(T, uint16_t, uint8_t)> setRemoteData;
    MetaBitfield metaData;
    T defaultValue;
    IObject& object;

   public:
    RemoteSubObject(
        uint16_t index, uint8_t subindex, MetaBitfield metaData,
        uint8_t* defaultValue,
        std::function<T(uint8_t*, uint16_t, uint16_t, uint8_t)> getRemoteData,
        std::function<void(uint8_t*, uint16_t, uint16_t, uint8_t)>
            setRemoteData)
        : getRemoteData(getRemoteData),
          setRemoteData(setRemoteData),
          index(index),
          subindex(subindex),
          metaData(metaData),
          defaultValue(defaultValue) {}
    MetaBitfield getMetadata() { return metaData; }
    uint16_t getSize() { return sizeof(T); }
    uint8_t* getDefaultValue() { return defaultValue; }
    void getData(uint8_t* buffer, uint16_t size) {
        getRemoteData(buffer, sizeof(T), object.getIndex(), subindex);
    }
    void setData(uint8_t* buffer, uint16_t size) {
        setRemoteData(buffer, sizeof(T), object.getIndex(), subindex);
    }
};

class IObject {
   private:
    SDOAbortCodes beforeSetData(uint8_t subIndex, uint8_t* buffer,
                                uint16_t sizeBytes);
    void afterSetData(uint8_t subIndex, uint8_t* buffer, uint16_t sizeBytes);
    SDOAbortCodes beforeGetData(uint8_t subIndex, uint8_t* buffer,
                                uint16_t sizeBytes);
    void afterGetData(uint8_t subIndex, uint8_t* buffer, uint16_t sizeBytes);

   public:
    virtual void getData(uint8_t subIndex, uint8_t* buffer,
                         uint16_t sizeBytes) = 0;
    virtual void setData(uint8_t subIndex, uint8_t* buffer,
                         uint16_t sizeBytes) = 0;
    virtual bool isSubValid(uint8_t subIndex) = 0;
    virtual uint16_t getSize(uint8_t subIndex) = 0;
    virtual uint16_t getSubObjectCount() = 0;
    virtual uint16_t getIndex() = 0;
};

template <typename T>
class LocalVarObject : public IObject {
   private:
    MetaBitfield metaData;
    T defaultValue;
    T value;
    uint16_t index;

   public:
    VarObject(uint16_t index, MetaBitfield metaData, T defaultValue)
        : index(index), metaData(metaData), defaultValue(defaultValue) {}
    void setData(uint8_t subIndex, uint8_t* buffer, uint16_t sizeBytes) {
        if (subIndex == 0) {
            memcpy(&value, buffer, sizeBytes);
        }
    }
    void getData(uint8_t subIndex, uint8_t* buffer, uint16_t sizeBytes) {
        if (subIndex == 0) {
            memcpy(buffer, &value, sizeBytes);
        }
    }
    bool isSubValid(uint8_t subIndex) { return subIndex == 0; }
    uint16_t getSize(uint8_t subIndex) { return sizeof(T); }
    uint16_t getSubObjectCount() { return 0; }
    uint16_t getIndex() { return index; }
    MetaBitfield getMetadata() { return metaData; }
};

template <typename T>
class RemoteVarObject : public IObject {
   private:
    std::function<T(uint8_t*, uint16_t, uint16_t, uint8_t)> getRemoteData;
    std::function<void(uint8_t*, uint16_t, uint16_t, uint8_t)> setRemoteData;
    MetaBitfield metaData;
    T defaultValue;
    T value;
    uint16_t index;

   public:
    VarObject(
        uint16_t index, MetaBitfield metaData, T defaultValue,
        std::function<T(uint8_t*, uint16_t, uint16_t, uint8_t)> getRemoteData,
        std::function<void(uint8_t*, uint16_t, uint16_t, uint8_t)>
            setRemoteData)
        : index(index),
          metaData(metaData),
          defaultValue(defaultValue),
          setRemoteData(setRemoteData),
          getRemoteData(getRemoteData) {}
    void setData(uint8_t subIndex, uint8_t* buffer, uint16_t size) {
        if (subIndex == 0) {
            setRemoteData(buffer, size, index, subIndex);
        }
    }
    void getData(uint8_t subIndex, uint8_t* buffer, uint16_t size) {
        if (subIndex == 0) {
            getRemoteData(buffer, size, index, subIndex);
        }
    }
    bool isSubValid(uint8_t subIndex) { return subIndex == 0; }
    uint16_t getSize(uint8_t subIndex) { return sizeof(T); }
    uint16_t getSubObjectCount() { return 0; }
    uint16_t getIndex() { return index; }
    MetaBitfield getMetadata() { return metaData; }
};

class RecordObject : public IObject {
   private:
    uint16_t index;
    uint16_t subObjectCount;
    ISubObject** subObjects;

   public:
    RecordObject(uint16_t index, uint16_t subObjectCount,
                 ISubObject** subObjects);
    void setData(uint8_t subIndex, uint8_t* buffer, uint16_t sizeBytes);
    void getData(uint8_t subIndex, uint8_t* buffer, uint16_t sizeBytes);
    bool isSubValid(uint8_t subIndex);
    uint16_t getSize(uint8_t subIndex);
    uint16_t getSubObjectCount();
    uint16_t getIndex();
    MetaBitfield getMetadata(uint8_t subIndex);
};

template <typename T>
class ArrayObject : public RecordObject {
   private:
    uint16_t index;
    uint16_t subObjectCount;
    ISubObject** subObjects;

   public:
    ArrayObject(uint16_t index, uint16_t subObjectCount,
                ISubObject** subObjects);
};

}  // namespace CANopen
