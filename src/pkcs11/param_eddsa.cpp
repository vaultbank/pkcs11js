#include "param.h"

void ParamEdDSA::FromV8(Local<Value> v8Value)
{
  Nan::HandleScope scope;

  try
  {
    if (!v8Value->IsObject())
    {
      THROW_ERROR("Parameter 1 MUST be Object", NULL);
    }

    Local<Object> v8Params = Nan::To<v8::Object>(v8Value).ToLocalChecked();

    // Check data
    if (!(check_param_empty(v8Params, STR_CONTEXT_DATA) || check_param_buffer(v8Params, STR_CONTEXT_DATA)))
      THROW_ERROR("Attribute 'contextData' MUST be NULL || BUFFER", NULL);

    Free();
    Init();

    v8::Local<v8::Value> v8PhFlag = Nan::Get(v8Params, Nan::New(STR_PH_FLAG).ToLocalChecked()).ToLocalChecked();

    if (!(v8PhFlag->IsNull() || v8PhFlag->IsBoolean())) {
			THROW_ERROR("Attribute 'phFlag' MUST be NULL || BOOLEAN", NULL);
		}

    CK_BBOOL phFlagPresent = v8PhFlag->IsBoolean() ? Nan::To<bool>(v8PhFlag).FromJust() : false;
    param.phFlag = phFlagPresent;

    if (!check_param_empty(v8Params, STR_CONTEXT_DATA))
    {
      GET_BUFFER_SMPL(buffer, Nan::To<v8::Object>(Nan::New(STR_CONTEXT_DATA).ToLocalChecked()).ToLocalChecked());
      param.pContextData = (CK_BYTE_PTR)malloc(bufferLen * sizeof(CK_BYTE));
      memcpy(param.pContextData, buffer, bufferLen);
      param.ulContextDataLen = (CK_ULONG)bufferLen;
    }
  }
  CATCH_ERROR;
}

void ParamEdDSA::Init()
{
  param = CK_EDDSA_PARAMS();
  param.phFlag = false;
  param.pContextData = NULL;
  param.ulContextDataLen = 0;
}

void ParamEdDSA::Free()
{
  if (param.pContextData)
  {
    free(param.pContextData);
    param.pContextData = NULL;
  }
}