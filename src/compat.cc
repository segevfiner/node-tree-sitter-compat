#include <napi.h>
#include <node.h>

typedef struct TSLanguage TSLanguage;

// "tree-sitter", "language" hashed with BLAKE2
const napi_type_tag LANGUAGE_TYPE_TAG = {
  0x8AF2E5212AD58ABF, 0xD5006CAD83ABBA16
};

// This module is an hack to convert a v8 API based language into a Node API
// external one, so it needs to convert a `napi_value` into a `v8::Object`, and
// due to using v8 API, is not a real pure Node API module.
//
// The following assertion and function were taken from Node's `napi`
// implementation:
// github.com/nodejs/node/blob/53ca0b9ae145c430842bf78e553e3b6cbd2823aa/src/js_native_api_v8.h
static_assert(
  sizeof(v8::Local<v8::Value>) == sizeof(napi_value),
  "Cannot convert between v8::Local<v8::Value> and napi_value"
);

static inline v8::Local<v8::Value> V8LocalValueFromJsValue(napi_value v) {
  v8::Local<v8::Value> local;
  memcpy(static_cast<void*>(&local), &v, sizeof(v));
  return local;
}

static inline void *GetInternalFieldPointer(Napi::Value value) {
  if (value.IsObject()) {
    v8::Local<v8::Object> object = v8::Local<v8::Object>::Cast(
      V8LocalValueFromJsValue(value)
    );
    if (object->InternalFieldCount() == 1) {
      return object->GetAlignedPointerFromInternalField(0);
    }
  }
  return nullptr;
}

static Napi::Value convertLanguage(const Napi::CallbackInfo& info) {
  auto env = info.Env();

  if (info.Length() < 1) {
    throw Napi::TypeError::New(env, "Wrong number of arguments");
  }

  Napi::Value value = info[0];
  if (value.IsObject()) {
    // Check for a napi language, and return it as is
    Napi::Value languageValue = value.As<Napi::Object>()["language"];
    auto arg = languageValue.As<Napi::External<TSLanguage>>();
    if (arg.IsExternal() && arg.CheckTypeTag(&LANGUAGE_TYPE_TAG)) {
      return value;
    }

    // Try to convert an old v8 API based language object
    auto language = static_cast<TSLanguage *>(GetInternalFieldPointer(value));
    if (language != nullptr) {
      auto languageObject = Napi::Object::New(env);
      languageObject["name"] = value.As<Napi::Object>().Get("name");
      auto languageExternal = Napi::External<TSLanguage>::New(env, language);
      languageExternal.TypeTag(&LANGUAGE_TYPE_TAG);
      languageObject["language"] = languageExternal;
      return languageObject;
    }
  }

  throw Napi::TypeError::New(env, "Invalid language object");
}

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
  Napi::Value process = env.Global()["process"];
  if (!process.IsObject()) {
    throw Napi::TypeError::New(env, "'process' is not an object");
  }

  Napi::Value versions = process.As<Napi::Object>()["versions"];
  if (!versions.IsObject()) {
    throw Napi::TypeError::New(env, "'versions' is not an object");
  }

  Napi::Value modulesVersionValue = versions.As<Napi::Object>()["modules"];
  auto modulesVersion = modulesVersionValue.ToNumber();

  // Since this module uses v8 API, don't allow to load it if is was built
  // agains't a different Node.js version, a check that would normally be done
  // by Node.js if the entry point was the classic node addon module entry point
  if (modulesVersion.Int32Value() != NODE_MODULE_VERSION) {
    throw Napi::TypeError::New(env, "The module was compiled against a different Node.js version");
  }

  exports.Set("convertLanguage", Napi::Function::New(env, convertLanguage));

  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
