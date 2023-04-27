// Ben Roberts

import SwiftUI

final class AddVaultVM: ObservableObject {
    @Published var vaultManager: VaultManagerM
    
    init() {
        self.vaultManager = VaultManagerM.shared
        checkForVaults()
    }
    
    internal func checkForVaults() {
        vaultManager.checkVaultsToConfigure()
    }
}
