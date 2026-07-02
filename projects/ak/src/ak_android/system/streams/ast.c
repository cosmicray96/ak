#include "ak/system/stream.h"
#include "ak/system/stream_itn.h"

#include "ak_android/android.h"

#include <android/asset_manager.h>
#include <android_native_app_glue.h>

ak_stmerr
ak_stm_open_ast(const char* path,
                ak_stm* o_stm)
{
  ak_stm stm = { 0 };
  stm.type = ak_stmtype_ast;

  struct android_app* app = ak_android_app();
  AAssetManager* mgr =
    app->activity->assetManager;

  AAsset* aasset = AAssetManager_open(
    mgr, path, AASSET_MODE_BUFFER);
  if (aasset == NULL) {
    return ak_stmerr_invalid;
  }
  stm.ctx = aasset;

  *o_stm = stm;
  return ak_stmerr_ok;
}

ak_stmerr
ak_stm_ast_close(void* ast)
{
  AAsset* aasset = ast;
  AAsset_close(aasset);
  return ak_stmerr_ok;
}

ak_stmresult
ak_stm_ast_write(void* ast,
                 const void* data,
                 uint64_t size)
{

  return (ak_stmresult){ .err =
                           ak_stmerr_err };
}

ak_stmresult
ak_stm_ast_read(void* ast,
                void* data,
                uint64_t size)
{
  AAsset* aasset = ast;
  int bytesRead =
    AAsset_read(aasset, data, size);

  ak_stmresult res = { 0 };
  res.transferred = bytesRead;
  if (bytesRead == size) {
    res.err = ak_stmerr_ok;
  } else {
    res.err = ak_stmerr_end;
  }
  return res;
}

ak_stmresult
ak_stm_ast_read_all(void* ast,
                    void** o_data,
                    uint64_t* o_size,
                    ak_alct alct)
{
  AAsset* aasset = ast;
  ak_stmresult r = { 0 };

  off_t size = AAsset_getLength(aasset);

  void* buf =
    ak_alct_alloc(alct, (uint64_t)size);

  int bytesRead =
    AAsset_read(aasset, buf, size);

  if ((size_t)bytesRead == (size_t)size) {
    r.err = ak_stmerr_ok;
    *o_data = buf;
    *o_size = (uint64_t)size;
  } else {
    r.err = ak_stmerr_err;
    ak_alct_free(alct, buf);
  }
  return r;
}
