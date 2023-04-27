// Ben Roberts

struct CheckUnconfiguredVaults: Encodable {
    /// ID of the user from this device
    let phoneID: String = UserM.shared.userID ?? ""
    /// Type of request for Arduino
    let requestType: String = "check setup"
}
