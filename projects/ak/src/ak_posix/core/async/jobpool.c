#include "ak/core/async/jobpool.h"

//===== ak_job =====//
//--- private ---//

//--- export ---//
ak_ex ak_job
ak_job_make(ak_job_fn job, void* ctx);
ak_ex ak_job_status
ak_job_get_status(ak_job* j);

//===== ak_jobpool =====//
//--- private ---//

//--- export ---//
ak_ex ak_jobpool*
ak_jobpool_startup();
ak_ex void
ak_jobpool_shutdown(ak_jobpool* jp);

ak_ex void
ak_jobpool_submit(ak_jobpool* jp, ak_job* j);
