// Ben Roberts

import SwiftUI

/// Basic storage for a vault door's info
class VaultM: Codable, Identifiable {
    /// Unique id for the vault
    let id: UUID
    /// Name of the vault
    let name: String
    /// 512 hash of user info to determine if the user is allowed to conenct to a vault
    let accessor: Int
}
