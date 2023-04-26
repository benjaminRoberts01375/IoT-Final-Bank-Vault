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
                ForEach(controller.availableVaults) { vault in
                    
                }
            }
            .navigationTitle("Add Vault")
        }
        .onAppear {
            controller.checkForVaults()
        }
    }
}

struct AddVaultV_Previews: PreviewProvider {
    static var previews: some View {
        AddVaultV()
    }
}
