// Ben Roberts

import SwiftUI

struct AddVaultV: View {
    @ObservedObject var controller: AddVaultVM
    
    init() {
        self._controller = ObservedObject(initialValue: AddVaultVM())
    }
    
    var body: some View {
        NavigationStack {
            List {
                ForEach(controller.vaultManager.vaultsToConfigure) { vault in
                    NavigationLink(destination: {
                        ConfigureVaultV(vault: vault)
                    }, label: {
                        Text("Vault - \(vault.id)")
                    })
                }
            }
            .navigationTitle("Add Vault")
        }
    }
}

struct AddVaultV_Previews: PreviewProvider {
    static var previews: some View {
        AddVaultV()
    }
}
