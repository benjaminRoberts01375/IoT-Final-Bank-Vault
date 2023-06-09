// Ben Roberts

struct RequestToSetupVault: Encodable {
    /// Vault to request from
    let vault: VaultM
    /// ID of the user from this device
    let phoneID: String = UserM.shared.userID ?? ""
    /// Type of request for Arduino
    let requestType: String = "request setup"
}
