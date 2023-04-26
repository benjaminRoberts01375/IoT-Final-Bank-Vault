// Ben Roberts

import SwiftUI

struct ConfigureVaultV: View {
    @StateObject var controller: ConfigureVaultVM

    init(vault: VaultM) {
        self._controller = StateObject(wrappedValue: ConfigureVaultVM(vault: vault))
    }
    
    var body: some View {
        NavigationStack {
            VStack {
                Spacer()
                HStack {
                    Spacer()
                    TextField("Name of vault", text: $controller.newVaultName)
                        .frame(width: 300)
                        .padding()
                        .background(.bar)
                        .cornerRadius(10)
                    Spacer()
                }
                Text("Vault ID: \(50000)")
                Spacer()
                NavigationLink(value: 0, label: {
                    Text("Save Name")
                        .foregroundColor(Color.primary)
                        .padding(8)
                        .background(.tertiary)
                        .cornerRadius(7)
                })
                Spacer()
            }
            .navigationTitle("Vault Setup")
        }
    }
}
