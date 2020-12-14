// Copyright 2020 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "generator/internal/connection_generator.h"
#include "absl/memory/memory.h"
#include "generator/internal/codegen_utils.h"
#include "generator/internal/descriptor_utils.h"
#include "generator/internal/predicate_utils.h"
#include "generator/internal/printer.h"
#include <google/api/client.pb.h>
#include <google/protobuf/descriptor.h>

namespace google {
namespace cloud {
namespace generator_internal {

ConnectionGenerator::ConnectionGenerator(
    google::protobuf::ServiceDescriptor const* service_descriptor,
    VarsDictionary service_vars,
    std::map<std::string, VarsDictionary> service_method_vars,
    google::protobuf::compiler::GeneratorContext* context)
    : ServiceCodeGenerator("connection_header_path", "connection_cc_path",
                           service_descriptor, std::move(service_vars),
                           std::move(service_method_vars), context) {}

Status ConnectionGenerator::GenerateHeader() {
  HeaderPrint(CopyrightLicenseFileHeader());
  HeaderPrint(  // clang-format off
    "// Generated by the Codegen C++ plugin.\n"
    "// If you make any local changes, they will be lost.\n"
    "// source: $proto_file_name$\n"
    "#ifndef $header_include_guard$\n"
    "#define $header_include_guard$\n"
    "\n");
  // clang-format on

  // includes
  HeaderLocalIncludes(
      {vars("idempotency_policy_header_path"), vars("stub_header_path"),
       vars("retry_traits_header_path"), "google/cloud/backoff_policy.h",
       "google/cloud/connection_options.h", "google/cloud/future.h",
       "google/cloud/internal/pagination_range.h",
       "google/cloud/polling_policy.h", "google/cloud/status_or.h",
       "google/cloud/version.h"});
  HeaderSystemIncludes({"google/longrunning/operations.grpc.pb.h", "memory"});
  HeaderPrint("\n");

  auto result = HeaderOpenNamespaces();
  if (!result.ok()) return result;

  // connection options
  HeaderPrint(  // clang-format off
    "struct $connection_options_traits_name$ {\n"
    "  static std::string default_endpoint();\n"
    "  static std::string user_agent_prefix();\n"
    "  static int default_num_channels();\n"
    "};\n\n");
  // clang-format on

  HeaderPrint(  // clang-format off
    "using $connection_options_name$ =\n"
    "  google::cloud::ConnectionOptions<$connection_options_traits_name$>;\n\n");
  // clang-format on

  HeaderPrint(  // clang-format off
    "using $retry_policy_name$ = google::cloud::internal::TraitBasedRetryPolicy<\n"
    "    $product_internal_namespace$::$retry_traits_name$>;\n"
    "\n"
    "using $limited_time_retry_policy_name$ = google::cloud::internal::LimitedTimeRetryPolicy<\n"
    "    $product_internal_namespace$::$retry_traits_name$>;\n"
    "\n"
    "using $limited_error_count_retry_policy_name$ =\n"
    "    google::cloud::internal::LimitedErrorCountRetryPolicy<\n"
    "        $product_internal_namespace$::$retry_traits_name$>;\n\n"
    //  clang-format on
  );

  // List*Range types
  for (auto const& method : methods()) {
    HeaderPrintMethod(
        method,
        {MethodPattern(
            {
                // clang-format off
   {"using $method_name$Range = "
    "google::cloud::internal::PaginationRange<\n"
    "    $range_output_type$>;\n\n"},
                // clang-format on
            },
            All(IsNonStreaming, Not(IsLongrunningOperation), IsPaginated))},
        __FILE__, __LINE__);
  }

  // Abstract interface Connection base class
  HeaderPrint(  // clang-format off
    "class $connection_class_name$ {\n"
    " public:\n"
    "  virtual ~$connection_class_name$() = 0;\n"
    "\n");
  // clang-format on

  for (auto const& method : methods()) {
    HeaderPrintMethod(
        method,
        {MethodPattern(
             {
                 {IsResponseTypeEmpty,
                  // clang-format off
    "  virtual Status\n",
    "  virtual StatusOr<$response_type$>\n"},
   {"  $method_name$($request_type$ const& request);\n"
        "\n",}
                 // clang-format on
             },
             All(IsNonStreaming, Not(IsLongrunningOperation),
                 Not(IsPaginated))),
         MethodPattern(
             {
                 {IsResponseTypeEmpty,
                  // clang-format off
    "  virtual future<Status>\n",
    "  virtual future<StatusOr<$longrunning_deduced_response_type$>>\n"},
   {"  $method_name$($request_type$ const& request);\n"
        "\n",}
                 // clang-format on
             },
             All(IsNonStreaming, IsLongrunningOperation, Not(IsPaginated))),
         MethodPattern(
             {
                 // clang-format off
   {"  virtual $method_name$Range\n"
    "  $method_name$($request_type$ request);\n\n"},
                 // clang-format on
             },
             All(IsNonStreaming, Not(IsLongrunningOperation), IsPaginated))},
        __FILE__, __LINE__);
  }

  // close abstract interface Connection base class
  HeaderPrint(  // clang-format off
    "};\n\n");
  // clang-format on

  HeaderPrint(  // clang-format off
    "std::shared_ptr<$connection_class_name$> Make$connection_class_name$(\n"
    "    $connection_options_name$ const& options = $connection_options_name$());\n\n");
  // clang-format on

  HeaderPrint(  // clang-format off
    "std::shared_ptr<$connection_class_name$> Make$connection_class_name$(\n"
    "    $connection_options_name$ const& options,\n"
    "    std::unique_ptr<$retry_policy_name$> retry_policy,\n"
    "    std::unique_ptr<BackoffPolicy> backoff_policy,\n"
    "    std::unique_ptr<PollingPolicy> polling_policy,\n"
    "    std::unique_ptr<$idempotency_class_name$> idempotency_policy);\n\n");
  // clang-format on

  HeaderPrint(  // clang-format off
    "std::shared_ptr<$connection_class_name$> Make$connection_class_name$(\n"
    "    std::shared_ptr<$product_internal_namespace$::$stub_class_name$> stub,\n"
    "    std::unique_ptr<$retry_policy_name$> retry_policy,\n"
    "    std::unique_ptr<BackoffPolicy> backoff_policy,\n"
    "    std::unique_ptr<PollingPolicy> polling_policy,\n"
    "    std::unique_ptr<$idempotency_class_name$> idempotency_policy);\n\n");
  // clang-format on

  HeaderCloseNamespaces();
  // close header guard
  HeaderPrint(  // clang-format off
    "#endif  // $header_include_guard$\n");
  // clang-format on
  return {};
}

Status ConnectionGenerator::GenerateCc() {
  CcPrint(CopyrightLicenseFileHeader());
  CcPrint(  // clang-format off
    "// Generated by the Codegen C++ plugin.\n"
    "// If you make any local changes, they will be lost.\n"
    "// source: $proto_file_name$\n\n");
  // clang-format on

  // includes
  CcLocalIncludes({vars("connection_header_path"),
                   vars("stub_factory_header_path"),
                   "google/cloud/internal/polling_loop.h",
                   "google/cloud/internal/retry_loop.h",
                   "google/cloud/internal/user_agent_prefix.h"});
  CcSystemIncludes({"memory"});
  CcPrint("\n");

  auto result = CcOpenNamespaces();
  if (!result.ok()) return result;

  CcPrint(  // clang-format off
    "std::string $connection_options_traits_name$::default_endpoint() {\n"
    "  return \"$service_endpoint$\";\n"
    "}\n\n");
  // clang-format on

  CcPrint(  // clang-format off
    "std::string $connection_options_traits_name$::user_agent_prefix() {\n"
    "  return google::cloud::internal::UserAgentPrefix();\n"
    "}\n\n"
    "int $connection_options_traits_name$::default_num_channels() { return 4; }\n\n");
  // clang-format on

  CcPrint(  // clang-format off
    "$connection_class_name$::~$connection_class_name$() = default;\n\n");
  // clang-format on

  for (auto const& method : methods()) {
    CcPrintMethod(
        method,
        {MethodPattern(
             {
                 {IsResponseTypeEmpty,
                  // clang-format off
    "Status\n",
    "StatusOr<$response_type$>\n"},
   {"$connection_class_name$::$method_name$(\n"
    "    $request_type$ const&) {\n"
    "  return Status(StatusCode::kUnimplemented, \"not implemented\");\n"
    "}\n\n"
    },
                 // clang-format on
             },
             All(IsNonStreaming, Not(IsLongrunningOperation),
                 Not(IsPaginated))),
         MethodPattern(
             {
                 {IsResponseTypeEmpty,
                  // clang-format off
    "future<Status>\n",
    "future<StatusOr<$longrunning_deduced_response_type$>>\n"},
   {"$connection_class_name$::$method_name$(\n"
    "    $request_type$ const&) {\n"
    "  return google::cloud::make_ready_future<\n"
    "    StatusOr<$longrunning_deduced_response_type$>>(\n"
    "    Status(StatusCode::kUnimplemented, \"not implemented\"));\n"
    "}\n\n"
    },
                 // clang-format on
             },
             All(IsNonStreaming, IsLongrunningOperation, Not(IsPaginated))),
         MethodPattern(
             {
                 // clang-format off
   {"$method_name$Range $connection_class_name$::$method_name$(\n"
    "    $request_type$ request) {\n"
    "  return google::cloud::internal::MakePaginationRange<$method_name$Range>(\n"
    "    std::move(request),\n"
    "    []($request_type$ const&) {\n"
    "      return StatusOr<$response_type$>{};\n"
    "    },\n"
    "    []($response_type$ const&) {\n"
    "      return std::vector<$range_output_type$>();\n"
    "    });\n"
    "}\n\n"
                     // clang-format on
                 },
             },
             All(IsNonStreaming, Not(IsLongrunningOperation), IsPaginated))},
        __FILE__, __LINE__);
  }

  // open anonymous namespace
  CcPrint("namespace {\n");

  // default policies
  CcPrint(  // clang-format off
    "std::unique_ptr<$retry_policy_name$> DefaultRetryPolicy() {\n"
    "  return $limited_time_retry_policy_name$(std::chrono::minutes(30)).clone();\n"
    "}\n"
    "\n"
    "std::unique_ptr<BackoffPolicy> DefaultBackoffPolicy() {\n"
    "  auto constexpr kBackoffScaling = 2.0;\n"
    "  return ExponentialBackoffPolicy(std::chrono::seconds(1),\n"
    "                                  std::chrono::minutes(5), kBackoffScaling)\n"
    "      .clone();\n"
    "}\n"
    "\n"
    "std::unique_ptr<PollingPolicy> DefaultPollingPolicy() {\n"
    "  auto constexpr kBackoffScaling = 2.0;\n"
    "  return GenericPollingPolicy<$limited_time_retry_policy_name$, ExponentialBackoffPolicy>(\n"
    "             $limited_time_retry_policy_name$(std::chrono::minutes(30)),\n"
    "             ExponentialBackoffPolicy(std::chrono::seconds(10),\n"
    "                                      std::chrono::minutes(5), kBackoffScaling))\n"
    "      .clone();\n"
    "}\n\n"
            // clang-format on
  );

  // default connection implementation class
  CcPrint(  //clang-format off
      "class $connection_class_name$Impl : public $connection_class_name$ {\n"
      " public:\n"
      "  explicit $connection_class_name$Impl(\n"
      "      std::shared_ptr<$product_internal_namespace$::$stub_class_name$> "
      "stub,\n"
      "      std::unique_ptr<$retry_policy_name$> retry_policy,\n"
      "      std::unique_ptr<BackoffPolicy> backoff_policy,\n"
      "      std::unique_ptr<PollingPolicy> polling_policy,\n"
      "      std::unique_ptr<$idempotency_class_name$> "
      "idempotency_policy)\n"
      "      : stub_(std::move(stub)),\n"
      "        retry_policy_prototype_(std::move(retry_policy)),\n"
      "        backoff_policy_prototype_(std::move(backoff_policy)),\n"
      "        polling_policy_prototype_(std::move(polling_policy)),\n"
      "        idempotency_policy_(std::move(idempotency_policy)) {}\n"
      "\n"
      "  explicit $connection_class_name$Impl(\n"
      "      std::shared_ptr<$product_internal_namespace$::$stub_class_name$> "
      "stub)\n"
      "      : $connection_class_name$Impl(\n"
      "          std::move(stub),\n"
      "          DefaultRetryPolicy(),\n"
      "          DefaultBackoffPolicy(),\n"
      "          DefaultPollingPolicy(),\n"
      "          MakeDefault$idempotency_class_name$()) {}\n"
      "\n"
      "  ~$connection_class_name$Impl() override = default;\n\n");
  //  clang-format on

  for (auto const& method : methods()) {
    CcPrintMethod(
        method,
        {MethodPattern(
             {
                 {IsResponseTypeEmpty,
                  // clang-format off
    "  Status\n",
    "  StatusOr<$response_type$>\n"},
   {"  $method_name$(\n"
    "      $request_type$ const& request) override {\n"
    "    return google::cloud::internal::RetryLoop(\n"
    "        retry_policy_prototype_->clone(), backoff_policy_prototype_->clone(),\n"
    "        idempotency_policy_->$method_name$(request),\n"
    "        [this](grpc::ClientContext& context,\n"
    "            $request_type$ const& request) {\n"
    "          return stub_->$method_name$(context, request);\n"
    "        },\n"
    "        request, __func__);\n"
    "}\n"
    "\n",}
                 // clang-format on
             },
             All(IsNonStreaming, Not(IsLongrunningOperation),
                 Not(IsPaginated))),
         MethodPattern(
             {
                 {IsResponseTypeEmpty,
                  // clang-format off
    "  future<Status>\n",
    "  future<StatusOr<$longrunning_deduced_response_type$>>\n"},
   {"  $method_name$(\n"
    "      $request_type$ const& request) override {\n"
    "    auto operation = google::cloud::internal::RetryLoop(\n"
    "        retry_policy_prototype_->clone(), backoff_policy_prototype_->clone(),\n"
    "        idempotency_policy_->$method_name$(request),\n"
    "        [this](grpc::ClientContext& context,\n"
    "               $request_type$ const& request) {\n"
    "          return stub_->$method_name$(context, request);\n"
    "        },\n"
    "        request, __func__);\n"
    "    if (!operation) {\n"
    "      return google::cloud::make_ready_future(\n"
    "          StatusOr<$longrunning_deduced_response_type$>(operation.status()));\n"
    "    }\n"
    "\n"
    "    return Await$method_name$(*std::move(operation));\n"
    "}\n"
    "\n",}
                 // clang-format on
             },
             All(IsNonStreaming, IsLongrunningOperation, Not(IsPaginated))),
         MethodPattern(
             {
                 // clang-format off
   {"  $method_name$Range $method_name$(\n"
    "      $request_type$ request) override {\n"
    "    request.clear_page_token();\n"
    "    auto stub = stub_;\n"
    "    auto retry =\n"
    "        std::shared_ptr<$retry_policy_name$ const>(retry_policy_prototype_->clone());\n"
    "    auto backoff = std::shared_ptr<BackoffPolicy const>(\n"
    "        backoff_policy_prototype_->clone());\n"
    "    auto idempotency = idempotency_policy_->$method_name$(request);\n"
    "    char const* function_name = __func__;\n"
    "    return google::cloud::internal::MakePaginationRange<$method_name$Range>(\n"
    "        std::move(request),\n"
    "        [stub, retry, backoff, idempotency, function_name]\n"
    "          ($request_type$ const& r) {\n"
    "          return google::cloud::internal::RetryLoop(\n"
    "              retry->clone(), backoff->clone(), idempotency,\n"
    "              [stub](grpc::ClientContext& context,\n"
    "                     $request_type$ const& request) {\n"
    "                return stub->$method_name$(context, request);\n"
    "              },\n"
    "              r, function_name);\n"
    "        },\n"
    "        []($response_type$ r) {\n"
    "          std::vector<$range_output_type$> result(r.$range_output_field_name$().size());\n"
    "          auto& messages = *r.mutable_$range_output_field_name$();\n"
    "          std::move(messages.begin(), messages.end(), result.begin());\n"
    "          return result;\n"
    "        });\n"
    "  }\n\n"
                     // clang-format on
                 },
             },
             All(IsNonStreaming, Not(IsLongrunningOperation), IsPaginated))},
        __FILE__, __LINE__);
  }

  CcPrint(  // clang-format off
    " private:\n");
  // clang-format on

  // TODO(#4038) - use the (implicit) completion queue to run this loop, and
  // once using a completion queue, consider changing to AsyncCancelOperation.
  CcPrint(  // clang-format off
    "  template <typename MethodResponse, template<typename> class Extractor,\n"
    "    typename Stub>\n"
    "  future<StatusOr<MethodResponse>>\n"
    "  AwaitLongrunningOperation(google::longrunning::Operation operation) {  // NOLINT\n"
    "    using ResponseExtractor = Extractor<MethodResponse>;\n"
    "    std::weak_ptr<Stub> cancel_stub(stub_);\n"
    "    promise<typename ResponseExtractor::ReturnType> pr(\n"
    "        [cancel_stub, operation]() {\n"
    "          grpc::ClientContext context;\n"
    "          context.set_deadline(std::chrono::system_clock::now() +\n"
    "            std::chrono::seconds(60));\n"
    "          google::longrunning::CancelOperationRequest request;\n"
    "          request.set_name(operation.name());\n"
    "          if (auto ptr = cancel_stub.lock()) {\n"
    "            ptr->CancelOperation(context, request);\n"
    "          }\n"
    "    });\n"
    "    auto f = pr.get_future();\n"
    "    std::thread t(\n"
    "        [](std::shared_ptr<Stub> stub,\n"
    "           google::longrunning::Operation operation,\n"
    "           std::unique_ptr<PollingPolicy> polling_policy,\n"
    "           google::cloud::promise<typename ResponseExtractor::ReturnType> promise,\n"
    "           char const* location) mutable {\n"
    "          auto result = google::cloud::internal::PollingLoop<ResponseExtractor>(\n"
    "              std::move(polling_policy),\n"
    "              [stub](grpc::ClientContext& context,\n"
    "                     google::longrunning::GetOperationRequest const& request) {\n"
    "                return stub->GetOperation(context, request);\n"
    "              },\n"
    "              std::move(operation), location);\n"
    "          stub.reset();\n"
    "          promise.set_value(std::move(result));\n"
    "        },\n"
    "        stub_, std::move(operation), polling_policy_prototype_->clone(),\n"
    "        std::move(pr), __func__);\n"
    "    t.detach();\n"
    "    return f;\n"
    "  }\n\n"
      );
  // clang-format on

  for (auto const& method : methods()) {
    CcPrintMethod(
        method,
        {MethodPattern(
            {
                {IsResponseTypeEmpty,
                 // clang-format off
    "  future<Status>\n",
    "  future<StatusOr<$longrunning_deduced_response_type$>>\n"},
   {"  Await$method_name$(\n"
    "      google::longrunning::Operation operation) {\n"
    "    return AwaitLongrunningOperation<\n"
    "        $longrunning_deduced_response_type$,\n"},
   {IsLongrunningMetadataTypeUsedAsResponse,
    "        google::cloud::internal::PollingLoopMetadataExtractor,\n",
    "        google::cloud::internal::PollingLoopResponseExtractor,\n"},
   {"        golden_internal::DatabaseAdminStub>(std::move(operation));\n"
    "  }\n\n"}
                // clang-format on
            },
            All(IsNonStreaming, IsLongrunningOperation, Not(IsPaginated)))},
        __FILE__, __LINE__);
  }

  CcPrint(  // clang-format off
    "  std::shared_ptr<$product_internal_namespace$::$stub_class_name$> stub_;\n"
    "  std::unique_ptr<$retry_policy_name$ const> retry_policy_prototype_;\n"
    "  std::unique_ptr<BackoffPolicy const> backoff_policy_prototype_;\n"
    "  std::unique_ptr<PollingPolicy const> polling_policy_prototype_;\n"
    "  std::unique_ptr<$idempotency_class_name$> idempotency_policy_;\n"
    "};\n");
  // clang-format on

  CcPrint("}  // namespace\n\n");

  CcPrint(  // clang-format off
    "std::shared_ptr<$connection_class_name$> Make$connection_class_name$(\n"
    "    $connection_options_name$ const& options) {\n"
    "  return std::make_shared<$connection_class_name$Impl>(\n"
    "      $product_internal_namespace$::CreateDefault$stub_class_name$(options));\n"
    "}\n\n");
  // clang-format on

  CcPrint(  // clang-format off
    "std::shared_ptr<$connection_class_name$> Make$connection_class_name$(\n"
    "    $connection_options_name$ const& options,\n"
    "    std::unique_ptr<$retry_policy_name$> retry_policy,\n"
    "    std::unique_ptr<BackoffPolicy> backoff_policy,\n"
    "    std::unique_ptr<PollingPolicy> polling_policy,\n"
    "    std::unique_ptr<$idempotency_class_name$> idempotency_policy) {\n"
    "  return std::make_shared<$connection_class_name$Impl>(\n"
    "      $product_internal_namespace$::CreateDefault$stub_class_name$(options),\n"
    "      std::move(retry_policy), std::move(backoff_policy),\n"
    "      std::move(polling_policy), std::move(idempotency_policy));\n"
    "}\n\n");
  // clang-format on

  CcPrint(  // clang-format off
    "std::shared_ptr<$connection_class_name$> Make$connection_class_name$(\n"
    "    std::shared_ptr<$product_internal_namespace$::$stub_class_name$> stub,\n"
    "    std::unique_ptr<$retry_policy_name$> retry_policy,\n"
    "    std::unique_ptr<BackoffPolicy> backoff_policy,\n"
    "    std::unique_ptr<PollingPolicy> polling_policy,\n"
    "    std::unique_ptr<$idempotency_class_name$> idempotency_policy) {\n"
    "  return std::make_shared<$connection_class_name$Impl>(\n"
    "      std::move(stub), std::move(retry_policy), std::move(backoff_policy),\n"
    "      std::move(polling_policy), std::move(idempotency_policy));\n"
    "}\n\n");
  // clang-format on

  CcCloseNamespaces();
  return {};
}

}  // namespace generator_internal
}  // namespace cloud
}  // namespace google
