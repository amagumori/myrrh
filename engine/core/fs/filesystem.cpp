// TODO

/*

#include "core/array.h"
#include "core/queue.h"
#include "file.h"
#include "disk_file_device.h"

namespace Engine {

  namespace FS {

    enum TransFlags {
      E_CLOSE = 0,
      E_SUCCESS = 0x1,
      E_IS_OPEN = E_SUCCESS << 1,
      E_FAIL = E_IS_OPEN << 1
    };

    struct AsyncItem {
      AsyncItem() {}
      File* _file;
      ReadCallback _cb;
      Mode _mode;
      char _path[MAX_PATH_LENGTH];
      uint8_t _flags;
    };

    static const int32_t C_MAX_TRANS = 16;

    typedef MT::Transaction<AsyncItem> AsyncTrans;
    typedef MT::LockFreeFixedQueue<AsyncTrans, C_MAX_TRANS> TransQueue;
    typedef Queue<AsyncTrans*, C_MAX_TRANS> InProgressQueue;
    typedef Array<AsyncItem> ItemsTable;
    typedef Array<FileDevice*> DevicesTable;

    class FSTask : public MT::Task {
      public:
        FSTask(TransQueue* queue, Allocator& allocator) :
          MT::Task(allocator),
          _trans_queue(queue)
        {}

        ~FSTask() {}

        int task() {
          while (!_trans_queue->isAborted()) {
            AsyncTrans* tr = _trans_queue->pop(true);
            if (!tr)
              break;
      
            if ((tr->data._flags & E_IS_OPEN) == E_IS_OPEN) {
              tr->data._flags |=
                tr->data._file->open(tr->data._path, tr->data._mode)
                ? E_SUCCESS
                : E_FAIL;
            } else if ((tr->data._flags & E_CLOSE) == E_CLOSE) {
              tr->data._file->close();
              tr->data._file->release();
            }
            tr->setCompleted();
          }
          return 0;
        }
      
        void stop() { _trans_queue->abort(); }
  
      private:
        TransQueue* _trans_queue;
    };

    class FileSystemImpl : public FileSystem {


    };
  }
}

*/
