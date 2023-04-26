// Ben Roberts

import Combine
import SwiftUI

final class VaultManagerM: ObservableObject {
    // Setup singleton
    public static var shared = VaultManagerM()
    private init() {
        self.vaultsToConfigure = []
    }
    
    /// Vaults in need of being configured
    @Published public var vaultsToConfigure: [VaultM]
}
