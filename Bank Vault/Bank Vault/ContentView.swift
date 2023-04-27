// Ben Roberts

import SwiftUI

struct ContentView: View {
    @StateObject var controller: ContentViewModel = ContentViewModel()
    
    var body: some View {
        NavigationStack {
            if UserM.shared.vaults.isEmpty {
                VStack {
                    Spacer()
                    Text("NO AVAILABLE VAULTS")
                        .font(.callout)
                        .fontWeight(.bold)
                        .foregroundColor(.secondary)
                }
            }
            else {
                List {
                    ForEach(Array(UserM.shared.vaults.enumerated()), id: \.element.id) { index, vault in
                        NavigationLink(destination: {
                            
                        }, label: {
                            Text(vault.name ?? "")
                        })
                        .transition(.opacity)
                    }
                }
            }
            Color.clear
            .edgesIgnoringSafeArea([.bottom, .horizontal])
            .navigationTitle("Your Vaults")
            .toolbar {
                if controller.editMode {
                    NavigationLink(                                 // Add vault button
                        destination: AddVaultV(),
                        label: {
                            Text("Add")
                        }
                    )
                    Button(action: {                                // Finish editing button
                        withAnimation {
                            controller.editMode.toggle()
                        }
                    }, label: {
                        Text("Done")
                    })
                    .disabled(UserM.shared.vaults.isEmpty)
                }
                else {
                    Button(action: {                                // Edit mode button
                        controller.editMode.toggle()
                    }, label: {
                        Text("Edit")
                    })
                }
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
