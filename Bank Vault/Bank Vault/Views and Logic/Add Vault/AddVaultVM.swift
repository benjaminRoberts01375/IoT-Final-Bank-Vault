// Ben Roberts

import SwiftUI

final class AddVaultVM: ObservableObject {
    @ObservedObject var vaultManager: VaultManagerM
    
    init() {
        self.vaultManager = VaultManagerM.shared
        checkForVaults()
    }
    
    internal func checkForVaults() {
        vaultManager.checkVaultsToConfigure()
    }
}
