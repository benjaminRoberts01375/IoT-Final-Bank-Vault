// Ben Roberts

import SwiftUI

/// Basic storage for a vault door's info
class VaultM: Codable, Identifiable, ObservableObject {
    /// Unique id for the vault
    let id: Int
    /// Name of the vault
    var name: String?
    /// ID of the user authorized to access the vault
    var accessor: String?
    /// History of the vault being interacted with
    var history: [ArduinoInteractionM] = []
}
