// Ben Roberts

import SwiftUI

struct VaultInfoV: View {
    @StateObject var controller: VaultInfoVM
    
    init(vault: VaultM) {
        self._controller = StateObject(wrappedValue: VaultInfoVM(vault: vault))
    }
    
    var body: some View {
        NavigationStack {
            VStack {
                List {
                    ForEach(controller.vault.history?.sorted(
                        by: { (lhs, rhs) -> Bool in
                            lhs.time < rhs.time
                        }
                    ) ?? []) { interaction in
                        if !interaction.isAllowedOpen && interaction.wasOpened {
                            Text("Security breach at \(interaction.time)")
                                .foregroundColor(.red)
                        }
                        else {
                            VStack(alignment: .leading) {
                                Text("Time: \(interaction.time)")
                                    .font(.callout)
                                    .foregroundColor(.secondary)
                                Text("Vault was \(interaction.isAllowedOpen ? "" : "not ")allowed to be open, and was\(interaction.wasOpened ? "" : "n't") open.")
                            }
                        }
                    }
                }
            }
            .navigationTitle("Vault - #\(controller.vault.id)")
        }
    }
}
