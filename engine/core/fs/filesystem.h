#pragma once

namespace Engine {

  struct Devicelist {
    FileDevice* _devices[8];
  };

  class FileSystem abstract {
    public:
      static FileSystem* create(Allocator& allocator);
      static void destroy(FileSystem* fs);
    
      FileSystem() {}
      virtual ~FileSystem() {}
      virtual bool mount(FileDevice* device) = 0;
      virtual bool unmount(FileDevice* device) = 0;

      virtual File* open(const DeviceList& device_list, const char* file, Mode mode) = 0;
      virtual bool openAsync(const DeviceList& device_list,
                             const char* file,
                             int mode,
                             const ReadCallback& callback) = 0;
      virtual void close(File& file) = 0;
      virtual void closeAsync(File& file) = 0;
      virtual void updateAsyncTransactions() = 0;
      virtual void fillDeviceList(const char* dev, DeviceList& device_list) = 0;
      virtual const DeviceList& getDefaultDevice() const = 0;
      virtual const DeviceList& getSaveGameDevice() const = 0;
      virtual const DeviceList& getMemoryDevice() const = 0;
      virtual const DeviceList& getDiskDevice() const = 0;

      virtual void setDefaultDevice(const char* dev) = 0;
      virtual void setSaveGameDevice(const char* dev) = 0;
  };









}
