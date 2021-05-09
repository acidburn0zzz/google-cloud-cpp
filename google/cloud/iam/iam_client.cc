// Copyright 2021 Google LLC
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

// Generated by the Codegen C++ plugin.
// If you make any local changes, they will be lost.
// source: google/iam/admin/v1/iam.proto

#include "google/cloud/iam/iam_client.h"
#include <memory>

namespace google {
namespace cloud {
namespace iam {
inline namespace GOOGLE_CLOUD_CPP_GENERATED_NS {

IAMClient::IAMClient(std::shared_ptr<IAMConnection> connection)
    : connection_(std::move(connection)) {}
IAMClient::~IAMClient() = default;

StreamRange<::google::iam::admin::v1::ServiceAccount>
IAMClient::ListServiceAccounts(std::string const& name) {
  ::google::iam::admin::v1::ListServiceAccountsRequest request;
  request.set_name(name);
  return connection_->ListServiceAccounts(request);
}

StatusOr<::google::iam::admin::v1::ServiceAccount> IAMClient::GetServiceAccount(
    std::string const& name) {
  ::google::iam::admin::v1::GetServiceAccountRequest request;
  request.set_name(name);
  return connection_->GetServiceAccount(request);
}

StatusOr<::google::iam::admin::v1::ServiceAccount>
IAMClient::CreateServiceAccount(
    std::string const& name, std::string const& account_id,
    ::google::iam::admin::v1::ServiceAccount const& service_account) {
  ::google::iam::admin::v1::CreateServiceAccountRequest request;
  request.set_name(name);
  request.set_account_id(account_id);
  *request.mutable_service_account() = service_account;
  return connection_->CreateServiceAccount(request);
}

Status IAMClient::DeleteServiceAccount(std::string const& name) {
  ::google::iam::admin::v1::DeleteServiceAccountRequest request;
  request.set_name(name);
  return connection_->DeleteServiceAccount(request);
}

StatusOr<::google::iam::admin::v1::ListServiceAccountKeysResponse>
IAMClient::ListServiceAccountKeys(
    std::string const& name,
    std::vector<
        ::google::iam::admin::v1::ListServiceAccountKeysRequest::KeyType> const&
        key_types) {
  ::google::iam::admin::v1::ListServiceAccountKeysRequest request;
  request.set_name(name);
  *request.mutable_key_types() = {key_types.begin(), key_types.end()};
  return connection_->ListServiceAccountKeys(request);
}

StatusOr<::google::iam::admin::v1::ServiceAccountKey>
IAMClient::GetServiceAccountKey(
    std::string const& name,
    ::google::iam::admin::v1::ServiceAccountPublicKeyType public_key_type) {
  ::google::iam::admin::v1::GetServiceAccountKeyRequest request;
  request.set_name(name);
  request.set_public_key_type(public_key_type);
  return connection_->GetServiceAccountKey(request);
}

StatusOr<::google::iam::admin::v1::ServiceAccountKey>
IAMClient::CreateServiceAccountKey(
    std::string const& name,
    ::google::iam::admin::v1::ServiceAccountPrivateKeyType private_key_type,
    ::google::iam::admin::v1::ServiceAccountKeyAlgorithm key_algorithm) {
  ::google::iam::admin::v1::CreateServiceAccountKeyRequest request;
  request.set_name(name);
  request.set_private_key_type(private_key_type);
  request.set_key_algorithm(key_algorithm);
  return connection_->CreateServiceAccountKey(request);
}

Status IAMClient::DeleteServiceAccountKey(std::string const& name) {
  ::google::iam::admin::v1::DeleteServiceAccountKeyRequest request;
  request.set_name(name);
  return connection_->DeleteServiceAccountKey(request);
}

StatusOr<::google::iam::v1::Policy> IAMClient::GetIamPolicy(
    std::string const& resource) {
  ::google::iam::v1::GetIamPolicyRequest request;
  request.set_resource(resource);
  return connection_->GetIamPolicy(request);
}

StatusOr<::google::iam::v1::Policy> IAMClient::SetIamPolicy(
    std::string const& resource, ::google::iam::v1::Policy const& policy) {
  ::google::iam::v1::SetIamPolicyRequest request;
  request.set_resource(resource);
  *request.mutable_policy() = policy;
  return connection_->SetIamPolicy(request);
}

StatusOr<::google::iam::v1::TestIamPermissionsResponse>
IAMClient::TestIamPermissions(std::string const& resource,
                              std::vector<std::string> const& permissions) {
  ::google::iam::v1::TestIamPermissionsRequest request;
  request.set_resource(resource);
  *request.mutable_permissions() = {permissions.begin(), permissions.end()};
  return connection_->TestIamPermissions(request);
}

StreamRange<::google::iam::admin::v1::Role> IAMClient::QueryGrantableRoles(
    std::string const& full_resource_name) {
  ::google::iam::admin::v1::QueryGrantableRolesRequest request;
  request.set_full_resource_name(full_resource_name);
  return connection_->QueryGrantableRoles(request);
}

StreamRange<::google::iam::admin::v1::ServiceAccount>
IAMClient::ListServiceAccounts(
    ::google::iam::admin::v1::ListServiceAccountsRequest request) {
  return connection_->ListServiceAccounts(std::move(request));
}

StatusOr<::google::iam::admin::v1::ServiceAccount> IAMClient::GetServiceAccount(
    ::google::iam::admin::v1::GetServiceAccountRequest const& request) {
  return connection_->GetServiceAccount(request);
}

StatusOr<::google::iam::admin::v1::ServiceAccount>
IAMClient::CreateServiceAccount(
    ::google::iam::admin::v1::CreateServiceAccountRequest const& request) {
  return connection_->CreateServiceAccount(request);
}

StatusOr<::google::iam::admin::v1::ServiceAccount>
IAMClient::PatchServiceAccount(
    ::google::iam::admin::v1::PatchServiceAccountRequest const& request) {
  return connection_->PatchServiceAccount(request);
}

Status IAMClient::DeleteServiceAccount(
    ::google::iam::admin::v1::DeleteServiceAccountRequest const& request) {
  return connection_->DeleteServiceAccount(request);
}

StatusOr<::google::iam::admin::v1::UndeleteServiceAccountResponse>
IAMClient::UndeleteServiceAccount(
    ::google::iam::admin::v1::UndeleteServiceAccountRequest const& request) {
  return connection_->UndeleteServiceAccount(request);
}

Status IAMClient::EnableServiceAccount(
    ::google::iam::admin::v1::EnableServiceAccountRequest const& request) {
  return connection_->EnableServiceAccount(request);
}

Status IAMClient::DisableServiceAccount(
    ::google::iam::admin::v1::DisableServiceAccountRequest const& request) {
  return connection_->DisableServiceAccount(request);
}

StatusOr<::google::iam::admin::v1::ListServiceAccountKeysResponse>
IAMClient::ListServiceAccountKeys(
    ::google::iam::admin::v1::ListServiceAccountKeysRequest const& request) {
  return connection_->ListServiceAccountKeys(request);
}

StatusOr<::google::iam::admin::v1::ServiceAccountKey>
IAMClient::GetServiceAccountKey(
    ::google::iam::admin::v1::GetServiceAccountKeyRequest const& request) {
  return connection_->GetServiceAccountKey(request);
}

StatusOr<::google::iam::admin::v1::ServiceAccountKey>
IAMClient::CreateServiceAccountKey(
    ::google::iam::admin::v1::CreateServiceAccountKeyRequest const& request) {
  return connection_->CreateServiceAccountKey(request);
}

StatusOr<::google::iam::admin::v1::ServiceAccountKey>
IAMClient::UploadServiceAccountKey(
    ::google::iam::admin::v1::UploadServiceAccountKeyRequest const& request) {
  return connection_->UploadServiceAccountKey(request);
}

Status IAMClient::DeleteServiceAccountKey(
    ::google::iam::admin::v1::DeleteServiceAccountKeyRequest const& request) {
  return connection_->DeleteServiceAccountKey(request);
}

StatusOr<::google::iam::v1::Policy> IAMClient::GetIamPolicy(
    ::google::iam::v1::GetIamPolicyRequest const& request) {
  return connection_->GetIamPolicy(request);
}

StatusOr<::google::iam::v1::Policy> IAMClient::SetIamPolicy(
    ::google::iam::v1::SetIamPolicyRequest const& request) {
  return connection_->SetIamPolicy(request);
}

StatusOr<::google::iam::v1::TestIamPermissionsResponse>
IAMClient::TestIamPermissions(
    ::google::iam::v1::TestIamPermissionsRequest const& request) {
  return connection_->TestIamPermissions(request);
}

StreamRange<::google::iam::admin::v1::Role> IAMClient::QueryGrantableRoles(
    ::google::iam::admin::v1::QueryGrantableRolesRequest request) {
  return connection_->QueryGrantableRoles(std::move(request));
}

StreamRange<::google::iam::admin::v1::Role> IAMClient::ListRoles(
    ::google::iam::admin::v1::ListRolesRequest request) {
  return connection_->ListRoles(std::move(request));
}

StatusOr<::google::iam::admin::v1::Role> IAMClient::GetRole(
    ::google::iam::admin::v1::GetRoleRequest const& request) {
  return connection_->GetRole(request);
}

StatusOr<::google::iam::admin::v1::Role> IAMClient::CreateRole(
    ::google::iam::admin::v1::CreateRoleRequest const& request) {
  return connection_->CreateRole(request);
}

StatusOr<::google::iam::admin::v1::Role> IAMClient::UpdateRole(
    ::google::iam::admin::v1::UpdateRoleRequest const& request) {
  return connection_->UpdateRole(request);
}

StatusOr<::google::iam::admin::v1::Role> IAMClient::DeleteRole(
    ::google::iam::admin::v1::DeleteRoleRequest const& request) {
  return connection_->DeleteRole(request);
}

StatusOr<::google::iam::admin::v1::Role> IAMClient::UndeleteRole(
    ::google::iam::admin::v1::UndeleteRoleRequest const& request) {
  return connection_->UndeleteRole(request);
}

StreamRange<::google::iam::admin::v1::Permission>
IAMClient::QueryTestablePermissions(
    ::google::iam::admin::v1::QueryTestablePermissionsRequest request) {
  return connection_->QueryTestablePermissions(std::move(request));
}

StatusOr<::google::iam::admin::v1::QueryAuditableServicesResponse>
IAMClient::QueryAuditableServices(
    ::google::iam::admin::v1::QueryAuditableServicesRequest const& request) {
  return connection_->QueryAuditableServices(request);
}

StatusOr<::google::iam::admin::v1::LintPolicyResponse> IAMClient::LintPolicy(
    ::google::iam::admin::v1::LintPolicyRequest const& request) {
  return connection_->LintPolicy(request);
}

}  // namespace GOOGLE_CLOUD_CPP_GENERATED_NS
}  // namespace iam
}  // namespace cloud
}  // namespace google