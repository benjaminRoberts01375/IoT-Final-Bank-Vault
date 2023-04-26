// Ben Roberts

import SwiftUI

/// Basic storage for a vault door's info
class VaultM: Codable, Identifiable, ObservableObject {
    /// Unique id for the vault
    let vaultID: Int
    /// Name of the vault
    var name: String?
    /// 512 hash of user info to determine if the user is allowed to conenct to a vault
    var accessor: Int?
}
