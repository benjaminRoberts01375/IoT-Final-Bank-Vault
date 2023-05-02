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
                HStack {
                    Spacer()
                        .overlay(
                            Button(action: {
                                controller.openDoor()
                            }, label: {
                                Text("Open")
                                    .frame(minWidth: 55, maxWidth: .infinity, minHeight: 55, maxHeight: 55)
                                    .background(.blue)
                                    .foregroundColor(.white)
                                    .cornerRadius(7)
                            })
                        )
                        .padding(.horizontal, 5)
                    Spacer()
                        .overlay(
                            Button(action: {
                                controller.closeDoor()
                            }, label: {
                                Text("Close")
                                    .frame(minWidth: 55, maxWidth: .infinity, minHeight: 55, maxHeight: 55)
                                    .background(.blue)
                                    .foregroundColor(.white)
                                    .cornerRadius(7)
                            })
                        )
                        .padding(.horizontal, 5)
                    Spacer()
                        .overlay(
                            Button(action: {
                                controller.addDevice()
                            }, label: {
                                Text("Enter Setup")
                                    .frame(minWidth: 55, maxWidth: .infinity, minHeight: 55, maxHeight: 55)
                                    .background(.blue)
                                    .foregroundColor(.white)
                                    .cornerRadius(7)
                            })
                        )
                        .padding(.horizontal, 5)
                    Spacer()
                        .overlay(
                            Button(action: {
                                controller.removeVault()
                            }, label: {
                                Text("Remove")
                                    .frame(minWidth: 55, maxWidth: .infinity, minHeight: 55, maxHeight: 55)
                                    .background(.blue)
                                    .foregroundColor(.white)
                                    .cornerRadius(7)
                            })
                        )
                        .padding(.horizontal, 5)
                }
                .padding(.vertical, 20)
                List {
                    ForEach(controller.vault.history?.sorted(
                        by: { (lhs, rhs) -> Bool in
                            lhs.time > rhs.time
                        }
                    ) ?? []) { interaction in
                        if !interaction.isAllowedOpen && interaction.wasOpened {
                            Text("Security breach at \(interaction.time)ms")
                                .foregroundColor(.red)
                        }
                        else {
                            VStack(alignment: .leading) {
                                Text("Time: \(interaction.time)ms")
                                    .font(.callout)
                                    .foregroundColor(.secondary)
                                Text("Vault was \(interaction.isAllowedOpen ? "" : "not ")allowed to be open, and was\(interaction.wasOpened ? "" : "n't") open.")
                                    .foregroundColor(interaction.isAllowedOpen == interaction.wasOpened ? .green : .orange)
                            }
                        }
                    }
                }
            }
            .navigationTitle("Vault - #\(controller.vault.id)")
        }
    }
}
