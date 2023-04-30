// Ben Roberts

import SwiftUI

final class VaultInfoVM: ObservableObject {
    let vault: VaultM
    
    init(vault: VaultM) {
        self.vault = vault
    }
}
