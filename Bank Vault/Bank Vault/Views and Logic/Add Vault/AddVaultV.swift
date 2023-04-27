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
                    HStack {
                        Text("Vault #\(vault.id)")
                        Spacer()
                        Button(action: {
                            controller.addVault(vault)
                        }, label: {
                            HStack {
                                Image(systemName: "plus.circle.fill")
                                Text("Add")
                            }
                            .padding(.horizontal, 12)
                            .padding(.vertical, 6)
                            .background(.quaternary)
                            .cornerRadius(20)
                        })
                    }
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
