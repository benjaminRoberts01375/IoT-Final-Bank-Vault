// Ben Roberts

/// Tracker for an interaction with a vault
class ArduinoInteractionM: Codable, Identifiable {
    /// Value for checking if the vault was allowed to be opened
    let isAllowedOpen: Bool
    /// Vault for checking if the vault was actually opened
    let wasOpened: Bool
    /// Time since vault creation
    let time: UInt32
}
