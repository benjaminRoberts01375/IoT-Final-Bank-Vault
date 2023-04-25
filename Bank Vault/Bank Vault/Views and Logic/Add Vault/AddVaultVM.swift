// Ben Roberts

import SwiftUI

final class AddVaultVM: ObservableObject {
    @Published var availableVaults: [VaultM]
    
    init() {
        availableVaults = []
    }
    
    internal func checkForVaults() {
        
    }
}
