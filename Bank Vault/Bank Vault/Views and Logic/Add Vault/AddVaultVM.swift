// Ben Roberts

import SwiftUI

final class AddVaultVM: ObservableObject {
    @Published var vaultManager: VaultManagerM
    
    init() {
        self.vaultManager = VaultManagerM.shared
    }
    
    internal func checkForVaults() {
        
    }
}
