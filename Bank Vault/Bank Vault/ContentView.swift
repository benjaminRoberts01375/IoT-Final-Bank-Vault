// Ben Roberts

import SwiftUI

struct ContentView: View {
    @StateObject var controller: ContentViewModel = ContentViewModel()
    
    var body: some View {
        NavigationStack {
            VStack {
                if VaultManagerM.shared.userVaults.isEmpty {
                    VStack {
                        Text("NO AVAILABLE VAULTS")
                            .font(.callout)
                            .fontWeight(.bold)
                            .foregroundColor(.secondary)
                    }
                }
                else {
                    List {
                        ForEach(Array(VaultManagerM.shared.userVaults.enumerated()), id: \.element.id) { index, vault in
                            NavigationLink(destination: {
                                VaultInfoV(vault: vault)
                            }, label: {
                                Text(vault.name ?? "Vault - #\(vault.id)")
                            })
                            .transition(.opacity)
                        }
                    }
                    .edgesIgnoringSafeArea([.bottom, .horizontal])
                }
            }
            .navigationTitle("Your Vaults")
            .toolbar {
                NavigationLink(                                 // Add vault button
                    destination: AddVaultV(),
                    label: {
                        Text("Add")
                    }
                )
            }
            .fullScreenCover(isPresented: $controller.showLoginPage) {
                UserLoginV()
            }
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}
