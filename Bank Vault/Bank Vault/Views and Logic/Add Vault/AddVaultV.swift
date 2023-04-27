// Ben Roberts

import SwiftUI

struct AddVaultV: View {
    @StateObject var controller: AddVaultVM
    
    init() {
        self._controller = StateObject(wrappedValue: AddVaultVM())
    }
    
    var body: some View {
        NavigationStack {
            List {
                ForEach(VaultManagerM.shared.vaultsToConfigure) { vault in
                    NavigationLink(destination: {
                        ConfigureVaultV(vault: vault)
                    }, label: {
                        Text("Vault - \(vault.id)")
                    })
                }
            }
            .navigationTitle("Add Vault")
            .toolbar(content: {
                Button(action: {                    // Refresh button
                    controller.checkForVaults()
                }, label: {
                    Text("Refresh")
                })
            })
        }
    }
}

struct AddVaultV_Previews: PreviewProvider {
    static var previews: some View {
        AddVaultV()
    }
}
