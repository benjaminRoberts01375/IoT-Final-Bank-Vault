// Ben Roberts

struct GenericVaultInteraction: Encodable {
    let vault: VaultM
    let interaction: String
    let phoneID: String = UserM.shared.userID ?? ""
}
