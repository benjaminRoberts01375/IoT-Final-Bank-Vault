// Ben Roberts

import SwiftUI

final class ConfigureVaultVM: ObservableObject {
    let vault: VaultM
    @Published var newVaultName: String
    
    init(vault: VaultM) {
        self.vault = vault
        self.newVaultName = "My New Vault"
    }
    
    deinit {
        self.vault.name = newVaultName
    }
}
